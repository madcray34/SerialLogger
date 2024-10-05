#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <iostream>
#include <view/RenderFileExplorer.hpp>


/**
 * @brief Here we define every aspect of what it should be drawn in the Frame window
 *
 * @param label
 */
void FileExplorer::Draw(std::string_view label)
{
   /**
    * @brief predefined ImGui flags to manage window properties, those will be passed down to the
    * ImGui::Begin function as 3rd argument
    */
   constexpr static auto window_flags = ImGuiWindowFlags_None;

   constexpr static auto windowSize = ImVec2(350.0F, 720.0F);
   constexpr static auto windowPos  = ImVec2(0.0F, 0.0F);

   /**
    * @brief Construct a new Im Gui:: Set Next Window Pos and Size objects to properly put into
    * settings the next "imgui sub-window" called with Begin()
    */
   // ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
   ImGui::SetNextWindowSize(windowSize);
   /*
    * @brief Start a new "imgui sub-windows" inside our main frame window (something that is render
    * inside) we must also call then an end window. This Begin, End work like a stack (in general
    * ImGui work like a stack) Whenever we want to create a new window we push data to stack, and at
    * the end the stack pointer must go to where it was before Begin.
    */
   ImGui::Begin(label.data(), nullptr, window_flags);
   /**
    * @brief In between Begin/End we have to create the content of this sub-window
    */
   DrawMenu();
   ImGui::Separator();    // Horizontal line
   DrawContent();
   ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 100.00F);    // set next element to position
   ImGui::Separator();
   DrawActions();
   ImGui::Separator();
   DrawFilter();
   ImGui::Separator();

   ImGui::End();
}

void FileExplorer::DrawMenu()
{
   if (ImGui::Button(c_goUpName.data()))
   {
      if (m_currentPath.has_parent_path())
      {
         m_currentPath = m_currentPath.parent_path();
      }
   }

   ImGui::SameLine();
   ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);    // Enable wrapping at the window's
                                                                // max width
   ImGui::Text(c_currentDir.data(), m_currentPath.string().c_str());
   ImGui::PopTextWrapPos();
}

void FileExplorer::DrawContent()
{
   for (const auto &entry : fs::directory_iterator(m_currentPath))
   {
      const auto &is_selected  = entry.path() == m_selectedEntry;
      const auto &is_directory = entry.is_directory();
      const auto &is_file      = entry.is_regular_file();

      std::string_view entry_name = entry.path().filename().string();

      // if (is_directory)
      // {
      //    entry_name = c_dirSymbol + entry_name;
      // }
      // else if (is_file)
      // {
      //    entry_name = c_fileSymbol + entry_name;
      // }

      // The boolean is for the refrashe rate to mantain highlighted the last selected item in the
      // current view
      if (ImGui::Selectable(entry_name.data(), is_selected))
      {
         if (is_directory)
         {
            m_currentPath /= entry.path().filename();
         }
         // The '/=' part add the fiolename to the currentpath for Unix use

         m_selectedEntry = entry.path();
      }
   }
}

void FileExplorer::DrawActions()
{
   ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);    // Enable wrapping at the window's
                                                                // max width
   if (fs::is_directory(m_selectedEntry))
   {
      ImGui::Text(c_selectedDir.data(), m_selectedEntry.string().c_str());
   }
   else if (fs::is_regular_file(m_selectedEntry))
   {
      ImGui::Text(c_selectedDir.data(), m_selectedEntry.string().c_str());
   }
   else
   {
      ImGui::Text(c_nothingSelected.data());
      // Since no button in this case is used ...
      // ... Fake a button to fix the size beneath
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha,    // it's invisible
                          ImGui::GetStyle().Alpha * 0.0f);
      ImGui::Button(c_nonClickable.data());
      ImGui::PopStyleVar();
      return;
   }

   if (fs::is_regular_file(m_selectedEntry) && ImGui::Button(c_open.data()))
   {
      openFileWithDefaultEditor();
   }

   ImGui::SameLine();

   if (ImGui::Button(c_rename.data()))
   {
      m_dlgRenameOpen = true;
      ImGui::OpenPopup(c_renamePopupName.data());
   }

   ImGui::SameLine();

   if (ImGui::Button(c_delete.data()))
   {
      m_dlgDeleteOpen = true;
      ImGui::OpenPopup(c_deletePopupName.data());
   }
   ImGui::PopTextWrapPos();
   renameFilePopup();
   deleteFilePopup();
}

void FileExplorer::DrawFilter()
{
   static char extention_filter[16] = { "\0" };
   float       availableWidth       = ImGui::GetContentRegionAvail().x - 5;
   ImGui::PushTextWrapPos(availableWidth);
   ImGui::Text(c_filter.data());
   ImGui::SameLine();
   ImGui::SetNextItemWidth(availableWidth);
   // The ### hides the label
   ImGui::InputText(c_maskedLabel.data(), extention_filter, sizeof(extention_filter));

   if (std::strlen(extention_filter) == 0)
   {
      return;
   }

   auto cnt = std::size_t{ 0 };
   for (const auto &entry : fs::directory_iterator(m_currentPath))
   {
      if (!fs::is_regular_file(entry))
         continue;

      if (entry.path().extension().string() == extention_filter)
      {
         ++cnt;
      }
   }

   ImGui::Text(c_numberOfLines.data(), cnt);
   ImGui::PopTextWrapPos();
}

void FileExplorer::openFileWithDefaultEditor()
{
#ifdef _WIN32
   const auto command = "start \"\" \"" + m_selectedEntry.string() + "\"";
#elif __APPLE__
   const auto command = "open \"" + m_selectedEntry.string() + "\"";
#else
   const auto command = "xdg-open \"" + m_selectedEntry.string() + "\"";
#endif

   std::system(command.c_str());
}

void FileExplorer::renameFilePopup()
{
   if (ImGui::BeginPopupModal(c_renamePopupName.data(), &m_dlgRenameOpen))
   {
      static char buffer_name[512] = { "\0" };

      ImGui::Text(c_newName.data());
      // ImGui::SetKeyboardFocusHere();
      ImGui::InputText(c_maskedLabel.data(), buffer_name, sizeof(buffer_name));
      if (ImGui::Button(c_rename.data()))
      {
         auto new_path = m_selectedEntry.parent_path() / buffer_name;
         if (renameFile(m_selectedEntry, new_path))
         {
            m_dlgRenameOpen = false;
            m_selectedEntry = new_path;
            std::memset(buffer_name, 0, sizeof(buffer_name));
         }
      }

      if (ImGui::Button(c_cancel.data()))
      {
         m_dlgRenameOpen = false;
      }

      ImGui::EndPopup();
   }
}

void FileExplorer::deleteFilePopup()
{
   if (ImGui::BeginPopupModal(c_deletePopupName.data(), &m_dlgDeleteOpen))
   {
      ImGui::Text(c_deleteQuestion.data(), m_selectedEntry.filename().string().c_str());
      if (ImGui::Button(c_yes.data()))
      {
         if (deleteFile(m_selectedEntry))
         {
            m_selectedEntry.clear();
            m_dlgDeleteOpen = false;
         }
      }

      if (ImGui::Button(c_no.data()))
      {
         m_dlgDeleteOpen = false;
      }
      ImGui::EndPopup();
   }
}

bool FileExplorer::renameFile(const fs::path &oldName, const fs::path &newName)
{
   try
   {
      fs::rename(oldName, newName);
      return true;
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
      return false;
   }
}

bool FileExplorer::deleteFile(const fs::path &name)
{
   try
   {
      fs::remove(name);
      return true;
   }
   catch (const std::exception &e)
   {
      std::cerr << e.what() << '\n';
      return false;
   }
}

void render(FileExplorer &window_obj)
{
   static constexpr auto viewName2 = "FileExplorer";
   window_obj.Draw(viewName2);
}
