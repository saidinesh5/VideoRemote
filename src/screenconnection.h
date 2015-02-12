#ifndef SCREENCONNECTION_H
#define SCREENCONNECTION_H

#include <QAbstractListModel>

class CommandSequence;
class QUdpSocket;
class QTimer;


/**
 * @brief The ScreenConnection class - This class sends/receives commands to the remote screen.
 * Currently this also plays the role of a sequencer.
 * But that can be refactored into it's own class to make this class "the network layer"
 * to comply with the KISS principle
 */
class ScreenConnection : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int currentSequenceId
               READ currentSequenceId
               WRITE setCurrentSequenceId
               NOTIFY currentSequenceIdChanged)

    Q_PROPERTY(CommandSequence* currentCommandSequence
               READ currentCommandSequence
               NOTIFY currentSequenceIdChanged)

    Q_PROPERTY(QString address
               READ address
               WRITE setAddress
               NOTIFY addressChanged)

    Q_PROPERTY(int port
               READ port
               WRITE setPort
               NOTIFY portChanged)

    Q_PROPERTY(bool running
               READ running
               NOTIFY runningChanged)

    Q_PROPERTY(int currentCommandChanged
               READ currentCommand
               WRITE setCurrentCommand
               NOTIFY currentCommandChanged)

public:
    explicit ScreenConnection(QObject *parent = 0);
    ~ScreenConnection();

    /// Standard boilerplate functions
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief Indicates the current CommandSequence id, from which we are sending commands to the remote screen
     * @return
     */
    int currentSequenceId() const;
    void setCurrentSequenceId(int id);

    CommandSequence *currentCommandSequence() const;

    /**
     * @brief The remote screen's IP address to send messages to
     * @return
     */
    QString address() const;
    void setAddress(const QString& address);

    /**
     * @brief The remote screen's Port number to send messages to
     * @return
     */
    int port() const;
    void setPort(int port);

    /**
     * @brief If the current sequence is sending messages
     * @return
     */
    bool running() const;

    /**
     * @brief How long has it been since the timer started
     * @return
     */
    int currentCommand() const;
    void setCurrentCommand(int index);

signals:
    void currentSequenceIdChanged();

    void addressChanged();
    void portChanged();

    void runningChanged();

    void currentCommandChanged();

public slots:
    /**
     * @brief Start sending commands from the current sequence
     */
    void start();

    /**
     * @brief Pause sending commands
     */
    void pause();

    /**
     * @brief reset the current CommandSequence to null and stop the timer...
     */
    void reset();

private slots:
    /**
     * @brief Whenever the timer ticks - send the next item from the current command sequence
     */
    void tick();

    /**
     * @brief nextTick
     */
    void nextTick();

    /**
     * @brief gets called whenever some remote sends us a message
     */
    void incoming();

private:
    enum {
        SequenceName = Qt::UserRole,
        SequenceObject = Qt::UserRole + 1
    };

    QUdpSocket* m_socket;

    QList<CommandSequence*> m_sequences;
    int m_currentSequenceId;

    QString m_address;
    int m_port;

    QTimer* m_timer;
    int m_currentCommand;
};

#endif // REMOTECONNECTION_H
