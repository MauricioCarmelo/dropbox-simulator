#!/bin/bash


# create folders
STAGE="stage_for_testing"

if [ ! -d $STAGE ]
then
    mkdir $STAGE
fi

# server folders
PRIMARY="$STAGE/primary_server"
SECONDARY1="$STAGE/secondary_server1"
SECONDARY2="$STAGE/secondary_server2"

# client folders
CLIENT1="$STAGE/client_1"
CLIENT2="$STAGE/client_2"

if [ ! -d $PRIMARY ]
then
    mkdir $PRIMARY
fi

if [ ! -d $SECONDARY1 ]
then
    mkdir $SECONDARY1
fi

if [ ! -d $SECONDARY2 ]
then
    mkdir $SECONDARY2
fi

if [ ! -d $CLIENT1 ]
then
    mkdir $CLIENT1
fi

if [ ! -d $CLIENT2 ]
then
    mkdir $CLIENT2
fi

# create server.txt files
SERVER_FILE="server.txt"

if [ ! -f "$PRIMARY/$SERVER_FILE" ]
then
    touch "$PRIMARY/$SERVER_FILE"
fi

if [ ! -f "$SECONDARY1/$SERVER_FILE" ]
then
    touch "$SECONDARY1/$SERVER_FILE"
fi

if [ ! -f "$SECONDARY2/$SERVER_FILE" ]
then
    touch "$SECONDARY2/$SERVER_FILE"
fi

# copy executables
BOX="box"
WAREHOUSE="warehouse"

if [ -f $BOX ]
then
    cp $BOX $CLIENT1
    cp $BOX $CLIENT2
fi

if [ -f $WAREHOUSE ]
then
    cp $WAREHOUSE $PRIMARY
    cp $WAREHOUSE $SECONDARY1
    cp $WAREHOUSE $SECONDARY2
fi