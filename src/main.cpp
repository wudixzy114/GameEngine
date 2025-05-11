#include "editor/Editor.h"
#include <spdlog/spdlog.h>

int startEditor(int argc, char *argv[]) {
    spdlog::info("func startEditor called");
    Editor &editor = Editor::getInstance(argc, argv);
    editor.start();
    return Editor::exec();
};

int main(int argc, char *argv[]) {
    startEditor(argc, argv);
}


