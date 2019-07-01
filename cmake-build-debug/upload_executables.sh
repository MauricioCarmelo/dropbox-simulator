# create folders
STAGE="stage_for_testing"

# server folders
PRIMARY="$STAGE/primary_server"
SECONDARY1="$STAGE/secondary_server1"
SECONDARY2="$STAGE/secondary_server2"

# client folders
CLIENT1="$STAGE/client_1"
CLIENT2="$STAGE/client_2"

if [ -f "$PRIMARY/warehouse" ]
then
    rm "$PRIMARY/warehouse"
    cp warehouse $PRIMARY
else
    cp warehouse $PRIMARY
fi

if [ -f "$SECONDARY1/warehouse" ]
then
    rm "$SECONDARY1/warehouse"
    cp warehouse $SECONDARY1
else
    cp warehouse $SECONDARY1
fi

if [ -f "$SECONDARY2/warehouse" ]
then
    rm "$SECONDARY2/warehouse"
    cp warehouse $SECONDARY2
else
    cp warehouse $SECONDARY2
fi

if [ -f "$CLIENT1/box" ]
then
    rm "$CLIENT1/box"
    cp box $CLIENT1
else
    cp box $CLIENT1
fi

if [ -f "$CLIENT2/box" ]
then
    rm "$CLIENT2/box"
    cp box $CLIENT2
else
    cp box $CLIENT2
fi