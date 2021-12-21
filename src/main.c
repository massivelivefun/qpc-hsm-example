#include "qpc.h"
#include "safe_std.h"
#include "states.h"
#include <stdlib.h> // for exit()

Q_DEFINE_THIS_FILE

int main() {
    QF_init();
    QF_onStartup();

    PRINTF_S("%s\n", "HSM Example:\n"
        "Press 'j' to switch larger states\n"
        "Press 'k' to switch smaller states in the second larger state\n"
        "Press 'ESC' to quit...");

    States_ctor();
    QHSM_INIT(states, (void *)0, 0U);
    
    for (;;) {
        QEvt e;
        uint8_t c;

        PRINTF_S("\n", "");
        c = (uint8_t)QF_consoleWaitForKey();
        PRINTF_S("%c: ", (c >= ' ') ? c : 'X');

        switch (c) {
            case 'j':   e.sig = J_CLICKED_SIG;  break;
            case 'k':   e.sig = K_CLICKED_SIG;  break;
            case 0x1B:  e.sig = TERMINATE_SIG;  break;
        }

        // dispatch the event into the state machine
        QHSM_DISPATCH(states, &e, 0U);
    }

    QF_onCleanup();
    return 0;
}

void QF_onStartup(void) {
    QF_consoleSetup();
}
void QF_onCleanup(void) {
    QF_consoleCleanup();
}
void QF_onClockTick(void) {}

Q_NORETURN Q_onAssert(const char * const module, const int_t location) {
    FPRINTF_S(stderr, "Assertion failed in %s, line %d", module, location);
    QF_onCleanup();
    exit(-1);
}
