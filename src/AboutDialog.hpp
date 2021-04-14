#ifndef QSYSTEMINFO_ABOUTDIALOG_HPP
#define QSYSTEMINFO_ABOUTDIALOG_HPP

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class AboutDialog;
}
QT_END_NAMESPACE

class AboutDialog : public QDialog
{
    Q_OBJECT
    public:
        explicit AboutDialog(QWidget *parent = nullptr);
        ~AboutDialog() override;

    private:
        Ui::AboutDialog *ui;
};

#endif //QSYSTEMINFO_ABOUTDIALOG_HPP
