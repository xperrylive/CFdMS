#ifndef ADMIN_H
#define ADMIN_H

// --- Main Menu ---
void adminMenu();

// --- Sub Menus ---
void userManagementMenu();
void applicationManagementMenu();
void backupRestoreMenu();

// --- User Management ---
void viewAllUsers();
void updateUser();
void deleteUser();

// --- Applications Management ---
void viewApplications();
void approveApplication();
void rejectApplication();

// --- Reports ---
void generateReports();

// --- Backup & Restore ---
void backupData();
void restoreData();

#endif
