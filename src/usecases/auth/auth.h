#ifndef __AUTH_H_
#define __AUTH_H_

#include "../../tads/user/user.h"

User* login(Avl* tree);
void signup(Avl** tree);

User* authUseCase(Avl** tree);
int usersUseCase(Avl** tree, User* user);
#endif