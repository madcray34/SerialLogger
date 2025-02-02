#pragma once
#include <string_view>
#include <filesystem>
#include <cstdint>

/**
 * @brief THIS FILE IS WORK IN PROGRESS... DO NOT CONSIDER IT FOR THE MOMENT
 *
 */
namespace fs = std::filesystem;

class FileExplorer
{
   public:
   FileExplorer(std::string_view defaultPath)
       : m_currentPath{ defaultPath }, m_selectedEntry{ fs::path() } {};
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

   const std::string_view c_renamePopupName{ "Rename File" };
   const std::string_view c_deletePopupName{ "Delete File" };
   const std::string_view c_goUpName{ "Go Up" };
   const std::string_view c_currentDir{ "Current directory: %s" };
   const std::string_view c_selectedDir{ "Selected file: %s" };
   const std::string_view c_nothingSelected{ "Nothing selected..." };
   const std::string_view c_nonClickable{ "Non clickable button" };
   const std::string_view c_rename{ "Rename" };
   const std::string_view c_delete{ "Delete" };
   const std::string_view c_open{ "Open" };
   const std::string_view c_cancel{ "Cancel" };
   const std::string_view c_newName{ "New name:   " };
   const std::string_view c_filter{ "Filter by extension" };
   const std::string_view c_maskedLabel{ "###masked" };
   const std::string_view c_numberOfLines{ "Number of files: %u" };
   const std::string_view c_dirSymbol{ "[D] %s" };
   const std::string_view c_fileSymbol{ "[F] %s" };
   const std::string_view c_deleteQuestion{ "Delete %s?" };
   const std::string_view c_yes{ "Yes" };
   const std::string_view c_no{ "No" };

   bool m_dlgRenameOpen{ false };
   bool m_dlgDeleteOpen{ false };
};

void render(FileExplorer &window_obj);
