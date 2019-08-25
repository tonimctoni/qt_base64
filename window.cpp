#include "window.hpp"
#include "base64encoderdecoder.hpp"
#include <QTextEdit>
#include <QApplication>

Window::Window(QWidget *parent) : QWidget(parent)
{
    setFixedSize(100,50);
    cleartext_textedit = new QTextEdit(this);
    cleartext_textedit->setGeometry(10,10,400,400);
    encoded_textedit = new QTextEdit(this);
    encoded_textedit->setGeometry(420,10,400,400);

    connect(cleartext_textedit, &QTextEdit::textChanged, this, &Window::please_encode);
    connect(encoded_textedit, &QTextEdit::textChanged, this, &Window::please_decode);
}

void Window::please_encode(){
    const auto text=cleartext_textedit->toPlainText();
    const auto text_as_utf8=text.toUtf8();
    const auto encoded_text=encode_base64(text_as_utf8.data(), text_as_utf8.size());

    QSignalBlocker b(encoded_textedit);
    emit encoded_textedit->setText(QString(encoded_text.c_str()));
}

void Window::please_decode(){
    const auto text=encoded_textedit->toPlainText();
    const auto text_as_utf8=text.toUtf8();
    std::string encoded_text;
    bool success;
    tie(success, encoded_text)=decode_base64(text_as_utf8.data(), text_as_utf8.size());
    // const auto [success,encoded_text]=decode_base64(text_as_utf8.data(), text_as_utf8.size());

    QSignalBlocker b(cleartext_textedit);
    if(success) emit cleartext_textedit->setText(QString(encoded_text.c_str()));
    else emit cleartext_textedit->setText(QString("Could not decode."));
}

