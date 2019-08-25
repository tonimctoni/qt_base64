#ifndef WINDOW_HEADER
#define WINDOW_HEADER
#include <QWidget>

class QTextEdit;

class Window : public QWidget
{
    Q_OBJECT
private:
    QTextEdit *cleartext_textedit;
    QTextEdit *encoded_textedit;
public:
    explicit Window(QWidget *parent=nullptr);
private slots:
    void please_encode();
    void please_decode();
};

#endif//WINDOW_HEADER
