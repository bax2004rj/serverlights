import curses

def startWindow(window):
    curses.start_color()
    curses.init_pair(1,curses.COLOR_BLUE,curses.COLOR_WHITE)
    window.bkgd(' ', curses.color_pair(1) | curses.A_BOLD | curses.A_REVERSE)
    window.addstr(1,1,"Serverlights Config Tool")
    running = True
    window.keypad(True)
    while running == True:
        key=window.getkey()
        if key == "q" or key == "Q":
            running = False

def main():
    curses.wrapper(startWindow)

main()