#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew UAssetEditor::MainForm(args));
}

namespace UAssetEditor 
{
    char* MainForm::to_pchar(String^ s)
    {
        System::Text::Encoding^ u8 = System::Text::Encoding::UTF8;
        array<unsigned char>^ bytes = u8->GetBytes(s);
        pin_ptr<unsigned char> pinnedPtr = &bytes[0];
        return (char*)pinnedPtr;
    }

    void MainForm::openFile(String^ filename)
    {
        if (!File::Exists(filename))
        {
            toolStripStatus->Text = "File doesn't exist.";
            return;
        }

        fileName = filename;
        Text = "UAssetEditor - " + Path::GetFileName(fileName);

        // Clear everything
        dataGridView->Enabled = false;
        dataGridView->Rows->Clear();
        saveToolStripMenuItem->Enabled = false;
        saveAsToolStripMenuItem->Enabled = false;
        toolStripStatus->Text = "";
        DELETE(header);
        DELETE(strings);
        DELETE(footer);

        // Open file
        char* s = to_pchar(filename);
        FILE* file = fopen(s, "rb");

        if (file == NULL)
        {
            toolStripStatus->Text = "Couldn't open the file.";
            return;
        }

        // Read header
        header = new char[HEADER_SIZE];
        int c = fread(header, 1, HEADER_SIZE, file);
        if (c != HEADER_SIZE)
        {
            fclose(file);
            DELETE(header);

            toolStripStatus->Text = "Couldn't read the header. Header section too short.";
            return;
        }

        int filesize = *(int*)(header + 24);
        int stringEnd = *(int*)(header + 69);
        // Other positions that change when changing file size
        //int pos1 = *(int*)(header + 61);
        //int pos2 = *(int*)(header + 73);
        //int pos3 = *(int*)(header + 165);
        //int pos4 = *(int*)(header + 169);
        //int pos5 = *(int*)(header + 189);
        //int pos6 = *(int*)(header + pos1 + 36); // file size again in the footer

        // Read strings
        stringsSize = stringEnd - HEADER_SIZE;
        strings = new char[stringsSize];

        if (fread(strings, 1, stringsSize, file) != stringsSize)
        {
            fclose(file);
            DELETE(header);
            DELETE(strings);

            toolStripStatus->Text = "Couldn't read the strings. String section too short.";
            return;
        }

        // Read footer
        footerSize = filesize - stringEnd;
        footer = new char[footerSize];

        if (fread(footer, 1, footerSize, file) != footerSize)
        {
            fclose(file);
            DELETE(header);
            DELETE(strings);
            DELETE(footer);

            toolStripStatus->Text = "Couldn't read the footer. Footer section too short.";
            return;
        }

        fclose(file);

        // Update the data grid
        char* p = strings;
        while (p < (strings + stringsSize))
        {
            // string length
            int l = *(int*)p;
            p = p + 4;
            // the string
            String^ str = gcnew String(p);
            p = p + l;
            // value after the string
            int i = dataGridView->Rows->Add(gcnew array<Object^> { str, * (unsigned int*)p });
            p = p + 4;
        }
        dataGridView->Enabled = true;

        saveToolStripMenuItem->Enabled = false;
        saveAsToolStripMenuItem->Enabled = true;
    }

    bool MainForm::saveFile(String^ filename)
    {
        // We write in a temporary file in case something fails
        char* s = to_pchar(filename + ".tmp");
        FILE* file = fopen(s, "wb+");

        if (file == NULL)
        {
            toolStripStatus->Text = "Error : couldn't open file.";
            return false;
        }

        // Find new size
        int newSize = 0;
        for (int i = 0; i < dataGridView->Rows->Count; i++)
        {
            newSize += ((String^)dataGridView->Rows[i]->Cells[0]->Value)->Length + 9;
        }

        // Header section
        char* newHeader = new char[HEADER_SIZE];
        memcpy(newHeader, header, HEADER_SIZE);

        int diff = newSize - stringsSize;

        // This is all the places where changing the length matter
        *(int*)(newHeader + 24) = *(int*)(header + 24) + diff; // File size
        *(int*)(newHeader + 61) = *(int*)(header + 61) + diff;
        *(int*)(newHeader + 69) = *(int*)(header + 69) + diff; // Strings end
        *(int*)(newHeader + 73) = *(int*)(header + 73) + diff;
        *(int*)(newHeader + 165) = *(int*)(header + 165) + diff;
        *(int*)(newHeader + 189) = *(int*)(header + 189) + diff;

        // Saving postion where to change footer
        int pos = *(int*)(newHeader + 61) - *(int*)(newHeader + 69) + 36;

        if (fwrite(newHeader, 1, HEADER_SIZE, file) != HEADER_SIZE)
        {
            fclose(file);
            DELETE(newHeader);

            toolStripStatus->Text = "Error : couldn't write header section.";
            return false;
        }

        DELETE(newHeader);

        // Strings section
        char* data = new char[newSize];
        char* p = data;
        for (int i = 0; i < dataGridView->Rows->Count; i++)
        {
            // string length
            int l = ((String^)dataGridView->Rows[i]->Cells[0]->Value)->Length + 1;
            *((int*)p) = l;
            p += 4;
            // the string
            char* s = to_pchar((String^)dataGridView->Rows[i]->Cells[0]->Value);
            memcpy(p, s, l);
            p += l;
            // the value after the string
            *((unsigned int*)p) = (unsigned int)dataGridView->Rows[i]->Cells[1]->Value;
            p += 4;
        }

        if (fwrite(data, 1, newSize, file) != newSize)
        {
            fclose(file);
            DELETE(data);

            toolStripStatus->Text = "Error : couldn't write strings section.";
            return false;
        }

        DELETE(data);

        // Footer section
        char* newFooter = new char[footerSize];
        memcpy(newFooter, footer, footerSize);

        *(int*)(newFooter + pos) = *(int*)(footer + pos) + diff; // Should be the file size

        if (fwrite(newFooter, 1, footerSize, file) != footerSize)
        {
            fclose(file);
            DELETE(newFooter);

            toolStripStatus->Text = "Error : couldn't write footer section.";
            return false;
        }
        fclose(file);
        DELETE(newFooter);

        try
        {
            // Nothing failed so we move/rename the temporary file
            if (File::Exists(filename)) File::Delete(filename);
            File::Move(filename + ".tmp", filename);
        }
        catch (const std::exception&) {}

        return true;
    }
}