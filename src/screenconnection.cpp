#include "screenconnection.h"
#include "commandsequence.h"

#include <QUdpSocket>
#include <QTimer>
#include <QDebug>

ScreenConnection::ScreenConnection(QObject *parent) :
    QAbstractListModel(parent),
    m_socket(new QUdpSocket(this)),
    m_currentSequenceId(-1),
    m_address("127.0.0.1"),
    m_port(10000),
    m_timer(new QTimer(this)),
    m_currentCommand(0)
{
    m_sequences << new CommandSequence("Sequence 1", this);
    m_sequences << new CommandSequence("Sequence 2", this);
    m_sequences << new CommandSequence("Sequence 3", this);

    m_timer->setSingleShot(true);
    connect( m_timer, SIGNAL(timeout()), this, SLOT(tick()) );
}

ScreenConnection::~ScreenConnection()
{

}

int ScreenConnection::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_sequences.count();
}

QHash<int, QByteArray> ScreenConnection::roleNames() const
{
    QHash<int, QByteArray> result;
    result[SequenceName] = "sequenceName";
    result[SequenceObject] = "sequenceObject";
    return result;
}

QVariant ScreenConnection::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
        case SequenceName:
            return m_sequences[index.row()]->name();

        case SequenceObject:
            return QVariant();//m_sequences[index.row()];
    }

    return QVariant();
}

QString ScreenConnection::address() const
{
    return m_address;
}

void ScreenConnection::setAddress(const QString &address)
{
    qDebug() << "ScreenConnection::setAddress" << address;
    if(m_address != address)
    {
        reset();

        m_address = address;
        emit addressChanged();
    }
}

int ScreenConnection::port() const
{
    return m_port;
}

void ScreenConnection::setPort(int port)
{
    qDebug() << "ScreenConnection::setPort" << port;
    if(m_port != port)
    {
        m_port = port;
        emit portChanged();
    }
}

int ScreenConnection::currentSequenceId() const
{
    return m_currentSequenceId;
}

void ScreenConnection::setCurrentSequenceId(int id)
{
    qDebug() << "ScreenConnection::setCurrentSequenceId" << id;
    reset();

    if(m_currentSequenceId != id)
    {
        m_currentSequenceId = id;
        emit currentSequenceIdChanged();
    }
}

CommandSequence *ScreenConnection::currentCommandSequence() const
{
    if(m_currentSequenceId >= 0 && m_currentSequenceId < m_sequences.count())
        return m_sequences[m_currentSequenceId];
    return nullptr;
}

bool ScreenConnection::running() const
{
    return m_timer->isActive();
}

int ScreenConnection::currentCommand() const
{
    return m_currentCommand;
}

void ScreenConnection::setCurrentCommand(int index)
{
    qDebug() << "ScreenConnection::setCurrentCommand" << index;
    if(m_currentCommand != index)
    {
        m_currentCommand = index;
        emit currentCommandChanged();
    }
}

void ScreenConnection::start()
{
    qDebug() << "ScreenConnection::start";
    CommandSequence* currentSequence = currentCommandSequence();
    if( !currentSequence )
    {
        qDebug() << "Cannot Start without a sequence selected";
        return;
    }

    //If we have to start from the beginning
    if(m_currentCommand < 0)
    {
        //Start the timer when we are supposed to start
        int time = currentSequence->timeToFire(0);
        if(time >= 0)
        {
            m_timer->start(time);
            setCurrentCommand(0);
        }
    }
    else tick(); // Else just continue from the current command
    emit runningChanged();
}

void ScreenConnection::pause()
{
    qDebug() << "ScreenConnection::pause";
    m_timer->stop();
    emit runningChanged();
}

void ScreenConnection::reset()
{
    qDebug() << "ScreenConnection::reset";
    setCurrentCommand(-1);
    m_timer->stop();
    emit runningChanged();
}

void ScreenConnection::tick()
{
    CommandSequence* currentSequence = currentCommandSequence();
    if(!currentSequence)
    {
        reset();
        return;
    }

    //Send the current message
    QByteArray message = currentSequence->messageToFire( m_currentCommand );
    if(!message.isEmpty())
        m_socket->writeDatagram(message, QHostAddress(m_address), m_port);

    nextTick();
}

void ScreenConnection::nextTick()
{
    CommandSequence* currentSequence = currentCommandSequence();
    if(!currentSequence)
    {
        reset();
        return;
    }

    //Figure out when to send the next message
    int presentTime = currentSequence->timeToFire( m_currentCommand );
    int nextTime = currentSequence->timeToFire( m_currentCommand + 1 );
    int delta = nextTime - presentTime;

    if(delta < 0) {
        reset();
        return;
    }

    m_timer->start(delta);
    setCurrentCommand( m_currentCommand + 1);
}

void ScreenConnection::incoming()
{

}
