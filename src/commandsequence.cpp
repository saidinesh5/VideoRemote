#include "commandsequence.h"

CommandSequence::CommandSequence(QString name, QObject *parent):
    QAbstractListModel(parent),
    m_name(name)
{
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SIGNAL(countChanged()));

    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SIGNAL(countChanged()));
}

CommandSequence::~CommandSequence()
{

}

int CommandSequence::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_commandList.count();
}

QHash<int, QByteArray> CommandSequence::roleNames() const
{
    QHash<int, QByteArray> result;
    result[Time] = "time";
    result[CommandId] = "commandId";
    result[CommandGroup] = "commandGroupId";
    result[CommandCode] = "commandCode";
    result[CommandOptions] = "commandOptions";
    return result;
}

QVariant CommandSequence::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const auto currentCommand = m_commandList[index.row()];

    switch(role) {
    case Time:
        return currentCommand.time;
    case CommandId:
        return currentCommand.commandId;
    case CommandGroup:
        return currentCommand.commandGroup;
    case CommandCode:
        return currentCommand.commandCode;
    case CommandOptions:
        return currentCommand.commandOptions;
    }

    return QVariant();
}

QVariant CommandSequence::get(const int i, int role) const
{
    return data(index(i), role);
}

int CommandSequence::timeToFire(int commandIndex)
{
    if(commandIndex >= 0 && commandIndex < m_commandList.count())
        return m_commandList[commandIndex].time;
    return -1;
}

QByteArray CommandSequence::messageToFire(int commandIndex)
{
    if(commandIndex >= 0 && commandIndex < m_commandList.count())
        return m_commandList[commandIndex].serialize();
    return QByteArray();
}

QString CommandSequence::name() const
{
    return m_name;
}

int CommandSequence::duration() const
{
    if(m_commandList.isEmpty())
        return 0;
    return m_commandList.last().time;
}

const QStringList CommandSequence::availableVideos() const
{
    return {"video1", "video2", "video3"};
}

const QStringList CommandSequence::availableEffects() const
{
    return {"fade", "powerup", "slide"};
}

bool CommandSequence::addEffect(int effectId, QString effectName, int startTime, int endTime)
{
    if(!availableEffects().contains(effectName))
        return false;

    if( addCommand( Command{ startTime, effectId, "effect", "start", effectName } ) &&
        addCommand( Command{ endTime, effectId, "effect", "stop", effectName } ) )
        emit effectAdded(effectId, effectName, startTime, endTime);

    return true;
}

bool CommandSequence::addVideo(int videoId, QString videoName, int startTime, int endTime)
{
    if(!availableVideos().contains(videoName))
        return false;

    if( addCommand( Command{ startTime, videoId, "video", "start", videoName } ) &&
        addCommand( Command{ endTime, videoId, "video", "stop", videoName } ) )
        emit videoAdded(videoId, videoName, startTime, endTime);

    return true;
}

bool CommandSequence::deleteEffect(int effectId)
{
    QList<int> indices;
    for(int i = 0; i < m_commandList.length(); i++)
        if(m_commandList[i].commandId == effectId && m_commandList[i].commandGroup == "effect")
            indices.push_back(i);

    for(int i = 0; i < indices.length(); i++)
        deleteCommand(indices[i] - i);

    if(!indices.isEmpty())
    {
        emit effectDeleted(effectId);
        return true;
    }

    return false;
}

bool CommandSequence::deleteVideo(int videoId)
{
    QList<int> indices;
    for(int i = 0; i < m_commandList.length(); i++)
        if(m_commandList[i].commandId == videoId && m_commandList[i].commandGroup == "video")
            indices.push_back(i);

    for(int i = 0; i < indices.length(); i++)
        deleteCommand(indices[i] - i);

    if(!indices.isEmpty())
    {
        emit videoDeleted(videoId);
        return true;
    }

    return false;
}

bool CommandSequence::addEntity(QString entityType, int entityId, QString entityName, int startTime, int endTime)
{
    if(entityType == "video")
        return addVideo(entityId, entityName, startTime, endTime);

    if(entityType == "effect")
        return addEffect(entityId, entityName, startTime, endTime);

    return false;
}

bool CommandSequence::deleteEntity(QString entityType, int entityId)
{
    QList<int> indices;
    for(int i = 0; i < m_commandList.length(); i++)
        if(m_commandList[i].commandId == entityId && m_commandList[i].commandGroup == entityType)
            indices.push_back(i);

    for(int i = 0; i < indices.length(); i++)
        deleteCommand(indices[i] - i);

    if(!indices.isEmpty())
    {
        emit entityDeleted(entityType, entityId);
        return true;
    }

    return false;
}

bool CommandSequence::addCommand(const CommandSequence::Command &command)
{
    int i = 0;

    // Find where to insert this in the list
    while(i < m_commandList.count() && m_commandList[i] < command) i++;

    beginInsertRows(QModelIndex(), i, i);
    m_commandList.insert(i, command);
    endInsertRows();

    return true;
}

bool CommandSequence::deleteCommand(const int i)
{
    if(i < 0 || i >= m_commandList.count())
        return false;

    beginRemoveRows(QModelIndex(), i, i);
    m_commandList.removeAt(i);
    endRemoveRows();

    return true;
}
