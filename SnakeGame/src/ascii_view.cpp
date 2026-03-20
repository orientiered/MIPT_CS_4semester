#include "ascii_view.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <string>

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <signal.h>

namespace sngm {

#define TTY_ESC "\033["

/* =================== Terminal window size change handler ==================== */

static bool g_has_window_changed = false; // used in pollEvent

static void sigWinchHandler(int sig) {
    g_has_window_changed = true;
}

/* ================== Implementation struct =================================== */

static std::pair<int, int> getTerminalSize() {
    struct winsize ws;

    const std::pair<int,int> default_sz = {50, 30};

    if (ioctl(STDOUT_FILENO,TIOCGWINSZ,&ws) < 0) {
        std::cerr << "Failed to get terminal size\n";
        return default_sz;
    }

    return {ws.ws_col, ws.ws_row};
}

template<typename Color>
struct ColorPalettePicker {
    std::vector<Color> palette;
    int idx = 0;

    ColorPalettePicker(std::vector<Color> p): palette(p) {}

    Color get() {
        Color c = palette[idx];
        idx = (idx + 1) % palette.size();
        return c;
    }

    void reset() { idx = 0; }
};

struct AsciiView::Impl {
    termios old_tty_attr;
    fd_set read_fds;

    std::queue<GameEvent> event_buffer;

    int tty_width = 50, tty_height = 30;

    const int height_bound = 2;
    const int width_bound = 2;

    int field_width = tty_width - width_bound, field_height = tty_height - height_bound;
    int field_start_x = 1, field_start_y = 1;

    const std::vector<int> snakePalette{112, 133, 172, 196, 27, 12, 215};
    ColorPalettePicker<int> snakeColor;

    Impl(): snakeColor(snakePalette) {

        //Initializing fd set for select
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        setupTerminal();
        setupWinchHandler();

        auto [new_tty_width, new_tty_height] = getTerminalSize();
        updateTerminalSize(new_tty_width, new_tty_height);
    }

    void setupTerminal() {
        // getting terminal attributes
        termios tty_attr;
        tcgetattr(STDIN_FILENO, &tty_attr);

        // saving them for restoring at destruction
        old_tty_attr = tty_attr;

        // modifying attrs
        // cfmakeraw(&tty_attr);
        tty_attr.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO, 0, &tty_attr);

        hideCursor();
    }

    void setupWinchHandler() {
        struct sigaction sa;

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = sigWinchHandler;
        if (sigaction(SIGWINCH, &sa, NULL) == -1)
            std::cerr << "Failed to setup winchHandler\n";

    }

    void clearScreen();
    void gotoXY(int x, int y);
    void gotoFieldXY(int x, int y);

    void setBgColor(int col_idx);
    void setFgColor(int col_idx);
    void clearColor();

    void hideCursor();
    void showCursor();

    void updateEventBuffer();
    void updateTerminalSize(int wx, int wy);

    void drawBox();
    void drawSnake(const Snake& snake);
    void drawRabbit(const Rabbit& rabbit);

    ~Impl() {
        showCursor();
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tty_attr);
    }
};


/* ============== Drawing primitives ================= */

void AsciiView::Impl::clearScreen() {
    printf(TTY_ESC "H" TTY_ESC "J");
}

void AsciiView::Impl::gotoXY(int x, int y) {
    printf(TTY_ESC"%d;%dH", y+1, x+1);
}

void AsciiView::Impl::gotoFieldXY(int x, int y) {
    gotoXY(x + field_start_x, field_height - 1 - y + field_start_y);
}

void AsciiView::Impl::hideCursor() {
    printf(TTY_ESC"?25l");
}

void AsciiView::Impl::showCursor() {
    printf(TTY_ESC"?25h");
}

void AsciiView::Impl::setFgColor(int col_idx) {
    printf(TTY_ESC"38;5;%dm", col_idx);
}

void AsciiView::Impl::setBgColor(int col_idx) {
    printf(TTY_ESC"48;5;%dm", col_idx);
}

void AsciiView::Impl::clearColor() {
    printf(TTY_ESC"0m");
}

// ============================================

void AsciiView::Impl::drawBox() {
    gotoXY(0, 0);

    int box_col = 110;

    setFgColor(box_col);
    for (int i = 0; i < tty_width; i++) {
        if (i == 0) printf("┌");
        else if (i == tty_width-1) printf("┐");
        else printf("─");
    }

    const int title_len = 7;
    gotoXY((tty_width-title_len) / 2, 0);
    setFgColor(162);
    printf(" Snake ");

    setFgColor(box_col);

    for (int y = 1; y < tty_height-1; y++) {
        gotoXY(0, y);
        printf("│");
        gotoXY(tty_width-1, y);
        printf("│");
    }

    gotoXY(0, tty_height-1);
    for (int i = 0; i < tty_width; i++) {
        if (i == 0) printf("└");
        else if (i == tty_width-1) printf("┘");
        else printf("─");
    }
}

void AsciiView::Impl::drawSnake(const Snake& snake) {
    setFgColor(snakeColor.get());

    const std::list<Coord>& body = snake.body;
    Coord head = body.front();

    const char* head_string = "^";
    switch(snake.direction) {
        case Direction::UP:
            head_string = "🠝";
            break;
        case Direction::DOWN:
            head_string = "🠟";
            break;
        case Direction::RIGHT:
            head_string = "🠞";
            break;
        case Direction::LEFT:
            head_string = "🠜";
            break;
    }

    gotoFieldXY(head.x, head.y);
    printf("%s", head_string);

    auto body_iter = body.begin();
    body_iter++;

    while (body_iter != body.end()) {

        gotoFieldXY(body_iter->x, body_iter->y);
        printf("#");
        body_iter++;
    }
}

void AsciiView::Impl::drawRabbit(const Rabbit& rabbit) {
    setFgColor(225);

    gotoFieldXY(rabbit.pos.x, rabbit.pos.y);
    printf("🤑");
    // printf("*");
}

void AsciiView::render(const GameModel& model) {
    impl_->clearScreen();
    impl_->drawBox();

    impl_->snakeColor.reset();
    for (const Snake& snake: model.snakes) {
        impl_->drawSnake(snake);
    }

    for (const Rabbit& rabbit: model.rabbits) {
        impl_->drawRabbit(rabbit);
    }

    std::cout << std::flush;
}

/* ================================================== */

void AsciiView::Impl::updateTerminalSize(int wx, int wy) {
    tty_width = wx;
    tty_height = wy;
    field_width = tty_width - width_bound;
    field_height = tty_height - height_bound;
}


void AsciiView::Impl::updateEventBuffer() {
    timeval timeout = {0, 0};
    fd_set set = read_fds;
    int ret = select(STDIN_FILENO+1, &set, 0, 0, &timeout);

    if (ret < 0) {
        std::cerr << "bad select\n";
        return;
    }
    if (ret == 0) {
        // std::cerr << "input timeout\n";
        return;
    }

    if (!FD_ISSET(STDIN_FILENO, &set)) {
        // std::cout << "No input in stdin\n";
        return;
    }

    char buffer[256];
    size_t n = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
    if (n > 0) {
        // std::cout << "processing " << n << " symbols from stdin\n";
    }

    struct key_str {
        std::string str;
        bool case_sensitive = false;
    };
    std::vector<std::pair<key_str, KeyEvent>> keys = {
        {{"q"}, KeyEvent::EXIT},
        {{"."}, KeyEvent::EXIT},
        {{"w"}, KeyEvent::P1_UP},
        {{"a"}, KeyEvent::P1_LEFT},
        {{"s"}, KeyEvent::P1_DOWN},
        {{"d"}, KeyEvent::P1_RIGHT},
        {{"\033[A", true}, KeyEvent::P2_UP},
        {{"\033[B", true}, KeyEvent::P2_DOWN},
        {{"\033[D", true}, KeyEvent::P2_LEFT},
        {{"\033[C", true}, KeyEvent::P2_RIGHT},
        {{"\t"}, KeyEvent::PAUSE},
        {{"r"}, KeyEvent::RESTART}
    };

    for (int i = 0; i < n; ) {
        int max_len = n-i;
        int match_idx = -1;
        const char *buf_pos = &buffer[i];

        for (int key_idx = 0; key_idx < keys.size(); key_idx++) {
            const char *str = keys[key_idx].first.str.c_str();
            const size_t len = keys[key_idx].first.str.size();

            bool case_sens = keys[key_idx].first.case_sensitive;

            if (len > max_len) continue;
            if ( case_sens && strncmp(str, buf_pos, len) == 0 ||
                !case_sens && strncasecmp(str, buf_pos, len) == 0) {
                match_idx = key_idx;
                i += len;
                event_buffer.push({keys[key_idx].second});
                break;
            }
        }

        if (match_idx < 0) i++; // no matches, skipping
    }
}

std::optional<GameEvent> AsciiView::pollEvent() {
    if (g_has_window_changed) {
        g_has_window_changed = false;

        auto [wx, wy] = getTerminalSize();
        impl_->updateTerminalSize(wx, wy);

        return GameEvent{WinchEvent{wx, wy}};
    }

    if (impl_->event_buffer.empty())
        impl_->updateEventBuffer();

    if (!impl_->event_buffer.empty()) {
        GameEvent event = impl_->event_buffer.front();
        impl_->event_buffer.pop();
        return event;
    } else {
        return std::nullopt;
    }

}

AsciiView::AsciiView(): impl_(std::make_unique<Impl>()) {}
AsciiView::~AsciiView() = default;


}
