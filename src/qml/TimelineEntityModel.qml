import QtQuick 2.0

ListModel {
    id: entityModel

    property int nextId: 0

    function addEntity( entityId, entityName, startTime, endTime )
    {
        for(var i = 0; i < entityModel.count; i++)
            if(entityModel.get(i).effectId === entityId)
                return false;

        entityModel.append({
                               'entityId': entityId,
                               'entityName': entityName,
                               'startTime': startTime,
                               'endTime' : endTime
                           })
        nextId++
        return true
    }

    function deleteEntity(entityId)
    {
        var indices = []
        for(var i = 0; i < entityModel.count; i++)
            if(entityModel.get(i).entityId === entityId)
                indices.push(i)

        for(var j = 0; j < indices.length; j++)
            entityModel.remove(indices[j] - j)

        if(indices.length > 0)
            return true
        return false
    }


// Format: ListElement { entityId: 0; entityName: 'powerup'; startTime: 100; endTime: 550 }
}

