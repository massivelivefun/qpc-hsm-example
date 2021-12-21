#include "qpc.h"
#include "safe_std.h"
#include "states.h"
#include <stdlib.h>

typedef struct {
/* protected */
    QHsm super;

/* private */
    int count;
} States;

static QState States_initial(States * const me, void const * const par);
static QState States_pause(States * const me, QEvt const * const e);
static QState States_cycle(States * const me, QEvt const * const e);
static QState States_first(States * const me, QEvt const * const e);
static QState States_second(States * const me, QEvt const * const e);
static QState States_third(States * const me, QEvt const * const e);
static QState States_final(States * const me, QEvt const * const e);

static States l_states;
QHsm * const states = &l_states.super;

void States_ctor(void) {
    States *me = &l_states;
    QHsm_ctor(&me->super, Q_STATE_CAST(&States_initial));
}

static void printCount(States * const me, char const * msg) {
    PRINTF_S("%s: %d\n", msg, me->count);
}

static void incCount(States * const me) {
    me->count += 1;
}

static void resetCount(States * const me) {
    me->count = 0;
}

static QState States_initial(States * const me, void const * const par) {
    (void)par; // unused transition parameter
    resetCount(me);
    printCount(me, "initial");
    return Q_TRAN(&States_pause);
}

static QState States_pause(States * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "entry-pause");
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            PRINTF_S("%s\n", "exit-pause");
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            PRINTF_S("%s\n", "init-pause");
            status_ = Q_HANDLED();
            break;
        }
        case J_CLICKED_SIG: {
            PRINTF_S("%s\n", "j-clicked-pause");
            status_ = Q_TRAN(&States_cycle);
            break;
        }
        case TERMINATE_SIG: {
            PRINTF_S("%s\n", "terminate-pause");
            status_ = Q_TRAN(&States_final);
            break;
        }
        default: {
            PRINTF_S("%s\n", "default-pause");
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

static QState States_cycle(States * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            resetCount(me);
            printCount(me, "cycle");
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&States_first);
            break;
        }
        case J_CLICKED_SIG: {
            PRINTF_S("%s\n", "j-clicked");
            status_ = Q_TRAN(&States_pause);
            break;
        }
        case TERMINATE_SIG: {
            PRINTF_S("%s\n", "terminate");
            status_ = Q_TRAN(&States_final);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
        }
    }
    return status_;
}

static QState States_first(States * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            incCount(me);
            printCount(me, "first");
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case K_CLICKED_SIG: {
            PRINTF_S("%s\n", "k-clicked-first");
            status_ = Q_TRAN(&States_second);
            break;
        }
        default: {
            status_ = Q_SUPER(&States_cycle);
            break;
        }
    }
    return status_;
}

static QState States_second(States * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            incCount(me);
            printCount(me, "second");
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case K_CLICKED_SIG: {
            PRINTF_S("%s\n", "k-clicked-second");
            status_ = Q_TRAN(&States_third);
            break;
        }
        default: {
            status_ = Q_SUPER(&States_cycle);
            break;
        }
    }
    return status_;
}

static QState States_third(States * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            incCount(me);
            printCount(me, "third");
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case K_CLICKED_SIG: {
            PRINTF_S("%s\n", "k-clicked-third");
            status_ = Q_TRAN(&States_first);
            break;
        }
        default: {
            status_ = Q_SUPER(&States_cycle);
            break;
        }
    }
    return status_;
}

static QState States_final(States * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "final");
            QF_onCleanup();
            exit(0);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
