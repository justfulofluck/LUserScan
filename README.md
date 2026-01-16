## 🔍 Linux User Info CLI Tool

This is a **robust C-based command-line tool** designed for system monitoring and information retrieval on Linux systems. It provides a user-friendly interface to query system users, groups, permissions, processes, and resource limits.

### ✅ Features:

- 🧑 **User Management**: List all system users or search for specific ones by name or UID.
- 👥 **Group Management**: List all groups, their members, and search for specific groups by name or GID.
- 🔐 **Membership Details**: Show all group memberships for any given user.
- 📂 **Advanced File Info**: Display permission details, ownership, and **recursive directory size** summaries.
- 👤 **Active Sessions**: List currently logged-in users with their terminal and login time.
- 🛠 **Resource Limits**: View system resource limits (ulimits) for the current process.
- 📈 **Process Viewer**: List all running processes owned by a specific user.
- 🎨 **Enhanced UI**: Clean, colorized terminal output for better readability.
- 🛡 **Robustness**: Safe input handling and dynamic memory management to handle systems of any size.

### ⚙️ How It Works

The tool utilizes standard Linux system APIs:
- `getpwent()`, `getgrent()`, `getpwnam()`, `getgrnam()`
- `getgrouplist()` (with dynamic allocation)
- `stat()` and `nftw()` (for recursive file walking)
- `getutxent()` (for session tracking)
- `getrlimit()` (for resource limits)
- `/proc` filesystem (for process inspection)

### 🛠 Installation on Ubuntu

Follow these steps to get the tool running on your Ubuntu system:

#### 1. Install Prerequisites
Ensure you have the build-essential package which includes the GCC compiler.
```bash
sudo apt update
sudo apt install build-essential
```

#### 2. Compile the Tool
Navigate to the project directory and run:
```bash
gcc user_info.c -o userinfo
```

#### 3. Run the Tool
```bash
./userinfo
```

#### 4. (Optional) Install Globally
If you want to run the tool from anywhere, move it to `/usr/local/bin`:
```bash
sudo mv userinfo /usr/local/bin/
```
Now you can simply run `userinfo` from any terminal.

### 🧪 Example Output:

```text
====== Linux User Information ======
1. List all Users
2. List all Groups
3. Show groups of a user
4. Show file or folder Permissions
5. Search User (Name or UID)
...
Enter your choice:
```

### 🔒 Note:
This tool is designed for **monitoring purposes**. It runs in user-space and does **not** modify any system settings. Some features (like viewing processes of other users) may provide limited information depending on your user permissions.
