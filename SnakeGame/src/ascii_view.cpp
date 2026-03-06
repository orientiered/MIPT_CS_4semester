#include "ascii_view.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <string>

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>

namespace sngm {

#define TTY_ESC "\033["

struct AsciiView::Impl {
    termios old_tty_attr;
    fd_set read_fds;

    std::queue<GameEvent> event_buffer;

    int width = 50, height = 30;


    Impl() {
        //Initializing fd set for select
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

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

    void clearScreen();
    void gotoXY(int x, int y);

    void setBgColor(int col_idx);
    void setFgColor(int col_idx);
    void clearColor();

    void hideCursor();
    void showCursor();

    void updateEventBuffer();

    void drawBox();

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
    for (int i = 0; i < width; i++) {
        if (i == 0) printf("┌");
        else if (i == width-1) printf("┐");
        else printf("─");
    }

    const int title_len = 7;
    gotoXY((width-title_len) / 2, 0);
    setFgColor(162);
    printf(" Snake ");

    setFgColor(box_col);

    for (int y = 1; y < height-1; y++) {
        gotoXY(0, y);
        printf("│");
        gotoXY(width-1, y);
        printf("│");
    }

    gotoXY(0, height-1);
    for (int i = 0; i < width; i++) {
        if (i == 0) printf("└");
        else if (i == width-1) printf("┘");
        else printf("─");
    }
}


void AsciiView::render(const GameModel& model) {
    impl_->clearScreen();
    impl_->drawBox();
    std::cout << std::flush;
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

    for (int i = 0; i < n; i++) {
        switch(buffer[i]) {
            case 'q': case '.':
                event_buffer.push({KeyEvent::EXIT});
                break;
            case 'w':
                event_buffer.push({KeyEvent::P1_UP});
                break;
            case 'a':
                event_buffer.push({KeyEvent::P1_LEFT});
                break;
            case 's':
                event_buffer.push({KeyEvent::P1_DOWN});
                break;
            case 'd':
                event_buffer.push({KeyEvent::P1_RIGHT});
                break;
            default:
                break;
        }
    }
}

std::optional<GameEvent> AsciiView::pollEvent() {
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
