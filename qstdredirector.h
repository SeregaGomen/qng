#ifndef QSTDREDIRECTOR_H
#define QSTDREDIRECTOR_H

#include <QTextEdit>
#include <QApplication>
#include <iostream>

class QStdSender : public QObject
{
    Q_OBJECT
public:
    explicit QStdSender(QObject* parent = 0): QObject(parent) {}
signals:
    void messageChanged(const QString & text);

protected:
    void printMessage(const char * ptr, int size)
    {
        qApp->processEvents();
        emit messageChanged(QByteArray(ptr, size));
    }
};

template< class Elem = char, class Tr = std::char_traits< Elem > >
class QStdRedirector : public QStdSender, public std::streambuf
{
public:
    QStdRedirector(std::ostream& a_Stream, QObject *parent = 0):
            QStdSender(parent), m_Stream(a_Stream)
    {
        m_pBuf = m_Stream.rdbuf( this ); // sets new stream
    }

    ~QStdRedirector()
    {
        m_Stream.rdbuf( m_pBuf ); // restores original stream
    }

    std::streamsize xsputn( const Elem* _Ptr, std::streamsize _Count )
    {
        printMessage( _Ptr, _Count );
        return _Count;
    }

    typename Tr::int_type overflow( typename Tr::int_type v )
    {
        Elem ch = Tr::to_char_type( v );
        printMessage( &ch, 1 );
        return Tr::not_eof( v );
    }

protected:
    std::basic_ostream<Elem, Tr>& m_Stream;
    std::streambuf*               m_pBuf;

};

#endif // QSTDREDIRECTOR_H
