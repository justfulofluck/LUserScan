## 🔍 Linux User Info CLI Tool

This is a **C-based command-line tool** that allows you to view user, group, and file permission information from a Linux system.

### ✅ Features:

- 🧑 List all system users (from `/etc/passwd`)
- 👥 List all groups and their members (from `/etc/group`)
- 🔐 Show all group memberships for a given user
- 📂 Display permission details of any file or folder
- 🧹 Interactive menu for easy-to-use navigation

### ⚙️ How It Works

The tool uses built-in Linux APIs like:

- `getpwent()` → to get users
- `getgrent()` → to get groups
- `getgrouplist()` → to find which groups a user belongs to
- `stat()` → to get file and directory permissions

The user interacts with a **text-based menu** through the terminal. Each option performs a system-level check and prints results in a human-readable format.

### 🧪 Example Output:

```
====== Linux User Information ======
1. List all Users
2. List all Groups
3. Show groups of a user
4. Show file or folder Permissions
5. Exit
Enter your choice:
```

### 🛠 Requirements:

- GCC compiler (Linux)
- Basic C programming knowledge (for modification)

### 🚀 How to Compile

```bash
gcc user_info.c -o userinfo
./userinfo
```

### 🔒 Note:
This is designed for **educational or system monitoring purposes only**. It runs in user-space and does **not** change any system settings or users.