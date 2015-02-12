#include <QtTest/QtTest>

#include "../src/commandsequence.h"

class TestCommandSequence: public QObject {
    Q_OBJECT
private:

private slots:
    void addCommand() {
        CommandSequence commandSequnece;
        QCOMPARE(commandSequnece.rowCount(), 0);

        //This command is supposed to be executed at 10msec
        commandSequnece.addCommand(CommandSequence::Command(10, 0, "video", "start", "video1"));
        QCOMPARE(commandSequnece.rowCount(), 1);

        //This command is supposed to be executed at 100msec
        commandSequnece.addCommand(CommandSequence::Command(100, 55, "video", "stop", "video1"));
        QCOMPARE(commandSequnece.rowCount(), 2);
        QCOMPARE(commandSequnece.data(commandSequnece.index(0), CommandSequence::CommandId ).toInt(), 0);
        QCOMPARE(commandSequnece.data(commandSequnece.index(1), CommandSequence::CommandId ).toInt(), 55);

        //This command is supposed to be executed at 50msec
        commandSequnece.addCommand(CommandSequence::Command(50, 250, "effect", "stop", "fade"));
        QCOMPARE(commandSequnece.rowCount(), 3);
        QCOMPARE(commandSequnece.data(commandSequnece.index(0), CommandSequence::CommandId ).toInt(), 0);
        QCOMPARE(commandSequnece.data(commandSequnece.index(1), CommandSequence::CommandId ).toInt(), 250);
        QCOMPARE(commandSequnece.data(commandSequnece.index(2), CommandSequence::CommandId ).toInt(), 55);

        //This command is supposed to be executed at 0msec
        commandSequnece.addCommand(CommandSequence::Command(0, 1000, "effect", "start", "fade"));
        QCOMPARE(commandSequnece.rowCount(), 4);
        QCOMPARE(commandSequnece.data(commandSequnece.index(0), CommandSequence::CommandId ).toInt(), 1000);
        QCOMPARE(commandSequnece.data(commandSequnece.index(1), CommandSequence::CommandId ).toInt(), 0);
        QCOMPARE(commandSequnece.data(commandSequnece.index(2), CommandSequence::CommandId ).toInt(), 250);
        QCOMPARE(commandSequnece.data(commandSequnece.index(3), CommandSequence::CommandId ).toInt(), 55);
    }

    void addVideo() {
        CommandSequence commandSequence;
        QCOMPARE(commandSequence.rowCount(), 0);

        //Fail to add a non existing video
        QVERIFY(!commandSequence.addVideo(5, "myvideo", 0, 100 ));
        QCOMPARE(commandSequence.rowCount(), 0);

        //Successfully add video1
        QVERIFY(commandSequence.addVideo(5, "video1", 0, 100 ));
        QCOMPARE(commandSequence.rowCount(), 2);
        QCOMPARE(commandSequence.data(commandSequence.index(0), CommandSequence::CommandCode ).toByteArray().data(), "start");
        QCOMPARE(commandSequence.data(commandSequence.index(1), CommandSequence::CommandCode).toByteArray().data(), "stop");

        //Successfully add video1
        QVERIFY(commandSequence.addVideo(55, "video2", 25, 80 ));
        QCOMPARE(commandSequence.rowCount(), 4);
        QCOMPARE(commandSequence.data(commandSequence.index(0), CommandSequence::CommandCode ).toByteArray().data(), "start");
        QCOMPARE(commandSequence.data(commandSequence.index(0), CommandSequence::CommandOptions ).toByteArray().data(), "video1");
        QCOMPARE(commandSequence.data(commandSequence.index(1), CommandSequence::CommandCode ).toByteArray().data(), "start");
        QCOMPARE(commandSequence.data(commandSequence.index(1), CommandSequence::CommandOptions ).toByteArray().data(), "video2");
        QCOMPARE(commandSequence.data(commandSequence.index(2), CommandSequence::CommandCode ).toByteArray().data(), "stop");
        QCOMPARE(commandSequence.data(commandSequence.index(2), CommandSequence::CommandOptions ).toByteArray().data(), "video2");
        QCOMPARE(commandSequence.data(commandSequence.index(3), CommandSequence::CommandCode ).toByteArray().data(), "stop");
        QCOMPARE(commandSequence.data(commandSequence.index(3), CommandSequence::CommandOptions ).toByteArray().data(), "video1");

        QCOMPARE(commandSequence.messageToFire(0).data(), "start video1\n");
        QCOMPARE(commandSequence.messageToFire(1).data(), "start video2\n");
        QCOMPARE(commandSequence.messageToFire(2).data(), "stop video2\n");
        QCOMPARE(commandSequence.messageToFire(3).data(), "stop video1\n");
    }

    void addEffect(){
        CommandSequence commandSequence;
        QCOMPARE(commandSequence.rowCount(), 0);

        //Fail to add a non existing video
        QVERIFY(!commandSequence.addEffect(5, "zoom", 0, 100 ));
        QCOMPARE(commandSequence.rowCount(), 0);

        //Successfully add video1
        QVERIFY(commandSequence.addEffect(5, "fade", 0, 100 ));
        QCOMPARE(commandSequence.rowCount(), 2);
        QCOMPARE(commandSequence.data(commandSequence.index(0), CommandSequence::CommandCode ).toByteArray().data(), "start");
        QCOMPARE(commandSequence.data(commandSequence.index(1), CommandSequence::CommandCode).toByteArray().data(), "stop");

        //Successfully add video1
        QVERIFY(commandSequence.addEffect(55, "powerup", 25, 80 ));
        QCOMPARE(commandSequence.rowCount(), 4);
        QCOMPARE(commandSequence.data(commandSequence.index(0), CommandSequence::CommandCode ).toByteArray().data(), "start");
        QCOMPARE(commandSequence.data(commandSequence.index(0), CommandSequence::CommandOptions ).toByteArray().data(), "fade");
        QCOMPARE(commandSequence.data(commandSequence.index(1), CommandSequence::CommandCode ).toByteArray().data(), "start");
        QCOMPARE(commandSequence.data(commandSequence.index(1), CommandSequence::CommandOptions ).toByteArray().data(), "powerup");
        QCOMPARE(commandSequence.data(commandSequence.index(2), CommandSequence::CommandCode ).toByteArray().data(), "stop");
        QCOMPARE(commandSequence.data(commandSequence.index(2), CommandSequence::CommandOptions ).toByteArray().data(), "powerup");
        QCOMPARE(commandSequence.data(commandSequence.index(3), CommandSequence::CommandCode ).toByteArray().data(), "stop");
        QCOMPARE(commandSequence.data(commandSequence.index(3), CommandSequence::CommandOptions ).toByteArray().data(), "fade");

        QCOMPARE(commandSequence.messageToFire(0).data(), "start fade\n");
        QCOMPARE(commandSequence.messageToFire(1).data(), "start powerup\n");
        QCOMPARE(commandSequence.messageToFire(2).data(), "stop powerup\n");
        QCOMPARE(commandSequence.messageToFire(3).data(), "stop fade\n");
    }
};

QTEST_MAIN(TestCommandSequence)
#include "testcommandsequence.moc"
