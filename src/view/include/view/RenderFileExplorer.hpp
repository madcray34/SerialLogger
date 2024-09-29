#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

class FileExplorer
{
public:
    FileExplorer()
        : m_currentPath{"C:/-- SomethingNew/ImGui/my_project/001 - "
                        "FileExplorer/dataToexperiment"},
          m_selectedEntry{fs::path()} {};
    ~FileExplorer() = default;
    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawContent();
    void DrawActions();
    void DrawFilter();
    void openFileWithDefaultEditor();

    void renameFilePopup();
    void deleteFilePopup();

    bool renameFile(const fs::path &oldName, const fs::path &newName);
    bool deleteFile(const fs::path &name);

private:
    fs::path m_currentPath;
    fs::path m_selectedEntry;

    const std::string_view c_renamePopupName{"Rename File"};
    const std::string_view c_deletePopupName{"Delete File"};

    bool m_dlgRenameOpen{false};
    bool m_dlgDeleteOpen{false};
};

void render(FileExplorer &window_obj);
