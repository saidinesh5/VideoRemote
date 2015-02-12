#ifndef COMMANDSEQUENCE_H
#define COMMANDSEQUENCE_H

#include <QAbstractListModel>

class CommandSequence: public QAbstractListModel
{

    Q_OBJECT

    Q_ENUMS(CommandAttributes)

    Q_PROPERTY(QStringList availableVideos
               READ availableVideos
               CONSTANT)

    Q_PROPERTY(QStringList availableEffects
               READ availableEffects
               CONSTANT)

    Q_PROPERTY(int count
               READ rowCount
               NOTIFY countChanged)

public:
    enum CommandAttributes {
        Time = Qt::UserRole,
        CommandId = Qt::UserRole + 1,
        CommandGroup = Qt::UserRole + 2,
        CommandCode = Qt::UserRole + 3,
        CommandOptions = Qt::UserRole + 4
    };

public:
    CommandSequence(QString name = "", QObject* parent = 0);
    ~CommandSequence();

    /// Standard boilerplate
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray>  roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    //Helper wrapper around data()
    Q_INVOKABLE QVariant get(const int i, int role) const;

    /**
     * @brief When to send the message of the command specified by the commandIndex
     * @param commandIndex
     * @return -1 upon failing to find a command with the given index
     */
    int timeToFire(int commandIndex);

    /**
     * @brief What to send as message of the command specified by the commandIndex
     * @param commandIndex
     * @return
     */
    QByteArray messageToFire(int commandIndex);

    /**
     * @brief name of this video sequence
     * @return
     */
    QString name() const;

    /**
     * @brief Total time needed to execute all the commands in this sequence
     * @return 0
     */
    int duration() const;

    /**
     * @brief availableVideos
     * @return
     */
    const QStringList availableVideos() const;

    /**
     * @brief availableEffects
     * @return
     */
    const QStringList availableEffects() const;

public slots:
    bool addEffect(int effectId, QString effectName, int startTime, int endTime);
    bool addVideo(int videoId, QString videoName, int startTime, int endTime);
    bool deleteEffect(int effectId);
    bool deleteVideo(int videoId);

    bool addEntity( QString entityType, int entityId, QString entityName, int startTime, int endTime);
    bool deleteEntity( QString entityType, int entityId );

signals:
    void videoAdded(int videoId, QString videoName, int startTime, int endTime);
    void effectAdded(int effectId, QString effectName, int startTime, int endTime);
    void videoDeleted(int videoId);
    void effectDeleted(int effectId);

    void entityDeleted(QString entityType, int entityId);

    void countChanged();

private:

    // Every UI action to be sent to the backend translates to one or more commands.
    // For eg. to play a video, the sequence of commands are:
    //   start video1 at 05
    //   stop video1 at 10
    struct Command {
        int time; // When to execute the command. Relative to the start, in milliseconds
        int commandId; // Each command has a unique id
        QString commandGroup; // Each command can belong to a UI group of commands. For eg. each video event has at least 2 commands: start, stop
        QString commandCode; // What command to be sent.
        QString commandOptions; // Additional options to be sent

        Command(int t = 0, int i = 0, QString group = "", QString command = "", QString options = ""):
            time(t),
            commandId(i),
            commandGroup(group),
            commandCode(command),
            commandOptions(options)
        {
        }

        QByteArray serialize() const
        {
            QByteArray result = QString(commandCode + " " + commandOptions + "\n").toLatin1();
            return result;
        }

        bool operator< (const Command& other) const
        {
            return this->time < other.time ||
                   (this->time == other.time && this->commandId < other.commandId);
        }
    };


    bool addCommand(const Command& command);
    bool deleteCommand(const int i);

    QString m_name;
    QList<Command> m_commandList; // The chronologically sorted list of commands

    friend class TestCommandSequence;
};

#endif // COMMANDSEQUENCE_H
