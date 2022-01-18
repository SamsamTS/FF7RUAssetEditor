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

        FileInfo^ info = gcnew FileInfo(filename);
        long long filesize = info->Length;

        // Open file
        char* s = to_pchar(filename);
        FILE* file = fopen(s, "rb");

        if (file == NULL)
        {
            toolStripStatus->Text = "Couldn't open the file.";
            return;
        }

        // Read header
        header = new uHeader;
        int c = fread(header, 1, HEADER_SIZE, file);
        if (c != HEADER_SIZE)
        {
            fclose(file);
            DELETE(header);

            toolStripStatus->Text = "Couldn't read the header. Header section too short.";
            return;
        }

        if (header->magic != UASSET_MAGIC)
        {
            fclose(file);
            DELETE(header);

            toolStripStatus->Text = "Wrong file format.";
            return;
        }

        if (header->NameOffset != HEADER_SIZE)
        {
            fclose(file);
            DELETE(header);

            toolStripStatus->Text = "Wrong header size.";
            return;
        }

        // Read strings
        stringsSize = header->ImportOffset - HEADER_SIZE;
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
        footerSize = filesize - header->ImportOffset;
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
        uHeader* newHeader = new uHeader;
        memcpy(newHeader, header, HEADER_SIZE);

        int diff = newSize - stringsSize;

        int o = header->ImportOffset - 1;
        if (header->SectionSixOffset > o)            newHeader->SectionSixOffset        = header->SectionSixOffset + diff;
        if (header->ExportOffset > o)                newHeader->ExportOffset            = header->ExportOffset + diff;
        if (header->ImportOffset > o)                newHeader->ImportOffset            = header->ImportOffset + diff;
        if (header->DependsOffset > o)               newHeader->DependsOffset           = header->DependsOffset + diff;
        if (header->SoftPackageReferencesOffset > o) newHeader->SoftPackageReferencesOffset = header->SoftPackageReferencesOffset + diff;
        if (header->SearchableNamesOffset > o)       newHeader->SearchableNamesOffset   = header->SearchableNamesOffset + diff;
        if (header->ThumbnailTableOffset > o)        newHeader->ThumbnailTableOffset    = header->ThumbnailTableOffset + diff;
        if (header->SearchableNamesOffset > o)       newHeader->SearchableNamesOffset   = header->SearchableNamesOffset + diff;
        if (header->AssetRegistryDataOffset > o)     newHeader->AssetRegistryDataOffset = header->AssetRegistryDataOffset + diff;
        if (header->BulkDataStartOffset > o)         newHeader->BulkDataStartOffset     = header->BulkDataStartOffset + diff;
        if (header->WorldTileInfoDataOffset > o)     newHeader->WorldTileInfoDataOffset = header->WorldTileInfoDataOffset + diff;
        if (header->PreloadDependencyOffset > o)     newHeader->PreloadDependencyOffset = header->PreloadDependencyOffset + diff;

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

        if (header->ExportOffset > 0)
        {
            char* p = newFooter + (header->ExportOffset - header->ImportOffset) + 36;
            for (int i = 0; i < header->ExportCount; i++)
            {
                // SerialOffset
                *(long long*)p= *(long long*)p + diff;
                p += 104;
            }
        }

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