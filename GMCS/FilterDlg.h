#ifndef FILTERDLG_H
#define FILTERDLG_H

#include <QDialog>
#include <QRegExp>
#include <QLabel>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QActionGroup>
#include <QKeyEvent>

Q_DECLARE_METATYPE(QRegExp::PatternSyntax)

class FilterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDlg(QWidget *parent = 0);

    void InitMainView();
    void ResetEditText();
protected:
    void keyPressEvent(QKeyEvent* keyEvent);

private:
    QLabel* m_pLableText;
    QComboBox* m_pComboText;

    QGroupBox* m_pGroupBox;
    QVBoxLayout* m_pVBLyout;
    QCheckBox* m_pCaseSensitivity;
    QComboBox* m_pComboPattern;

    QGridLayout* m_pMainLayout;
signals:
    void FilterChanged(QRegExp regExp);
public slots:
    void FilterRegExpChanged();

};

#endif // FILTERDLG_H
