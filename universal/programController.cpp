#include "programController.h"

QString getRegDitValue(QString RegDitPath, QString key){
    QSettings settings(RegDitPath, QSettings::NativeFormat); // HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399
    return settings.value(key).toString();
}

void runProgramAsAdmin(const QString &program, const QStringList &arguments) {
    // 将程序和参数转为 LPCWSTR 格式
    QString params = arguments.join(" ");
    LPCWSTR programPath = reinterpret_cast<LPCWSTR>(program.utf16());
    LPCWSTR paramsPath = reinterpret_cast<LPCWSTR>(params.utf16());

    // 调用 ShellExecute 来以管理员身份运行程序
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"runas";  // 以管理员身份运行
    sei.lpFile = programPath;  // 程序路径
    sei.lpParameters = paramsPath;  // 程序参数
    sei.nShow = SW_NORMAL;  // 窗口正常显示

    // 启动程序
    if (!ShellExecuteExW(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            // 用户取消了 UAC 提示
        } else {
            // 处理其他错误
        }
    }
}

std::string ws2s(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

DWORD findProcessByName(const std::string& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return DWORD();

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    // 遍历进程列表以查找进程名
    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            std::string exeFileName = ws2s(processEntry.szExeFile);  // 转换宽字符为 std::string
            if (processName == exeFileName) {
                DWORD processID = processEntry.th32ProcessID;
                CloseHandle(snapshot);
                return processID;
                break;
            }
        } while (Process32NextW(snapshot, &processEntry));
    }

    return DWORD();
}

void TerminateProcessByID(DWORD processID) {
    HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (processHandle) {
        if (TerminateProcess(processHandle, 0)) {
            std::cout << "Process terminated successfully." << std::endl;
        } else {
            std::cerr << "TerminateProcess failed. Error code: " << GetLastError() << std::endl;
        }
        CloseHandle(processHandle);
    } else {
        std::cerr << "Unable to open process." << std::endl;
    }
}

void TerminateProcessByName(const std::string& processName){
    DWORD processID = findProcessByName(processName);
    if(processID != DWORD()){
        // 递归终止进程树
        TerminateProcessTree(processID);
    }
    else{
        std::cout << "can't find process." << std::endl;
    }
}

bool TerminateProcessByNameAndCheck(const std::string& processName, int nums)
{
    for (int i = 0; i < nums; i++) {
        DWORD processID = findProcessByName(processName);
        if (processID != DWORD()) {
            TerminateProcessByID(processID);
        }
        else {
            return true;
        }
        QThread::msleep(3000);
    }
    return false;
}

// 获取指定进程的所有子进程ID
std::vector<DWORD> GetChildProcesses(DWORD parentPID) {
    std::vector<DWORD> childPIDs;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create snapshot. Error: " << GetLastError() << std::endl;
        return childPIDs;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ParentProcessID == parentPID) {
                childPIDs.push_back(pe32.th32ProcessID);
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    else {
        std::cerr << "Failed to retrieve process information. Error: " << GetLastError() << std::endl;
    }

    CloseHandle(hSnapshot);
    return childPIDs;
}

// 递归终止进程树
bool TerminateProcessTree(DWORD processID) {
    // 获取子进程
    std::vector<DWORD> childPIDs = GetChildProcesses(processID);

    // 递归终止子进程
    for (DWORD childPID : childPIDs) {
        TerminateProcessTree(childPID);
    }

    // 终止当前进程
    HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (processHandle) {
        if (TerminateProcess(processHandle, 0)) {
            std::cout << "Terminated process: " << processID << std::endl;
            CloseHandle(processHandle);
            return true;
        }
        else {
            std::cerr << "Failed to terminate process: " << processID << ". Error: " << GetLastError() << std::endl;
            CloseHandle(processHandle);
            return false;
        }
    }
    else {
        std::cerr << "Unable to open process: " << processID << ". Error: " << GetLastError() << std::endl;
        return false;
    }
}
