
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "../client-side/include/Box.h"

TEST_CASE("Test get_command_id() from class Box")
{
    // arrange
    Box box;
    char command1[] = "upload";
    char command2[] = "download";
    char command3[] = "exit";
    char command4[] = "get_sync_dir";

    // act
    auto result1 = box.get_command_id(command1);
    auto result2 = box.get_command_id(command2);
    auto result3 = box.get_command_id(command3);
    auto result4 = box.get_command_id(command4);

    // assert
    REQUIRE(result1 == UPLOAD);
    REQUIRE(result2 == DOWNLOAD);
    REQUIRE(result3 == EXIT);
    REQUIRE(result4 == GET_SYNC_DIR);
}

TEST_CASE("Test prepare_instruction() from class Box")
{
    // arrange
    Box box;

    Instruction inst1, inst2;

    char line1[] = "download arquivo.pdf";
    strcpy(inst1.command_name, "download");
    inst1.command_id = DOWNLOAD;
    strcpy(inst1.path, "");
    strcpy(inst1.filename, "arquivo.pdf");

    char line2[] = "upload pasta1/pasta2/arquivo.pdf";
    strcpy(inst2.command_name, "upload");
    inst2.command_id = UPLOAD;
    strcpy(inst2.path, "pasta1/pasta2/");
    strcpy(inst2.filename, "arquivo.pdf");
    // act
    auto result1 = box.prepare_instruction(line1);
    auto result2 = box.prepare_instruction(line2);




    // assert
    CHECK( strcmp(result1.command_name, inst1.command_name) == 0 );
    CHECK( result1.command_id == inst1.command_id );
    CHECK( strcmp(result1.path, inst1.path) == 0 );
    CHECK( strcmp(result1.filename, inst1.filename) == 0 );

    CHECK( strcmp(result2.command_name, inst2.command_name) == 0 );
    CHECK( result2.command_id == inst2.command_id );
    CHECK( strcmp(result2.path, inst2.path) == 0 );
    CHECK( strcmp(result2.filename, inst2.filename) == 0 );


}