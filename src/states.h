#ifndef STATES_H
#define STATES_H

enum ButtonSignals {
    J_CLICKED_SIG = Q_USER_SIG,
    K_CLICKED_SIG,
    TERMINATE_SIG,
};

extern QHsm * const states;

void States_ctor(void);

#endif
