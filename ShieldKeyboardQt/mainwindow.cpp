
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qt_windows.h>


HHOOK GobleWindowHook = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    on_BtnUninstallHook_clicked();
    delete ui;
}

LRESULT CALLBACK hookProc(int code, WPARAM wParam, LPARAM lParam){
    PKBDLLHOOKSTRUCT virKey = (PKBDLLHOOKSTRUCT)lParam;
    if(code>=0){
        // 按键消息
        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            switch (virKey->vkCode)
            {
            case VK_LWIN:
            case VK_RWIN:
                return 1;  // 吃掉消息
                break;
            }
            return 1;
            break;
        }
    }
    return CallNextHookEx(GobleWindowHook, code, wParam, lParam);
}

void MainWindow::on_BtnInstallHook_clicked()
{
    if(!GobleWindowHook){
        GobleWindowHook = SetWindowsHookExA(
                    WH_KEYBOARD_LL,
                    hookProc,
                    GetModuleHandleA(NULL),
                    NULL
                    );
    }
    if(!GobleWindowHook){
        QMessageBox::warning(NULL,QString("错误"),QString("屏蔽键盘消息失败:%d").arg(GetLastError()));
    }else{
        ui->BtnInstallHook->setEnabled(false);
        ui->BtnUninstallHook->setEnabled(true);
        ui->BtnInstallHook->setText(QString("√"));
    }
}

void MainWindow::on_BtnUninstallHook_clicked()
{
    if(GobleWindowHook){
            UnhookWindowsHookEx(GobleWindowHook);
            GobleWindowHook = NULL;
            ui->BtnInstallHook->setEnabled(true);
            ui->BtnInstallHook->setText(QString("屏蔽键盘"));
            ui->BtnUninstallHook->setEnabled(false);
        }
}
