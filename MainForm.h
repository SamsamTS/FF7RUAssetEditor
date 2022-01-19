#pragma once

#include <fstream>
#include <vcclr.h>

namespace UAssetEditor {

#define DELETE(x) {delete x; x = NULL;}
#define UASSET_MAGIC 2653586369

#pragma pack(push, 1)
	struct uHeader
	{
		unsigned int magic; //0
		int LegacyFileVersion; //4
		int LegacyUE3Version; //8
		int fileVersionUE4; //12
		int FileVersionLicenseeUE4; //16
		int numCustomVersions; //20
		int SectionSixOffset; //24
		int FolderNameLength; //28
		char FolderName[5]; //32
		unsigned int PackageFlags;//37
		int NameCount; //41
		int NameOffset; //45
		int GatherableTextDataCount; //49
		int GatherableTextDataOffset; //53
		int ExportCount; //57
		int ExportOffset; //61
		int ImportCount; //65
		int ImportOffset; //69
		int DependsOffset; //73
		int SoftPackageReferencesCount; //77
		int SoftPackageReferencesOffset; //81
		int SearchableNamesOffset; //85
		int ThumbnailTableOffset; //89
		char PackageGuid[16]; //93
		int generationCount; //109
		int genNumExports; //113
		int genNumNames; //117
		char RecordedEngineVersion[14];//121
		char RecordedCompatibleWithEngineVersion[14]; //135
		int CompressionFlags; //149 
		int numCompressedChunks; //153
		int PackageSource; //157
		int numAdditionalPackagesToCook; //161
		int AssetRegistryDataOffset; //165
		long long BulkDataStartOffset; //169
		int WorldTileInfoDataOffset; //177
		int numChunkIDs; //181
		int PreloadDependencyCount; //185
		int PreloadDependencyOffset; //189
	};
#pragma pack(pop)

#define HEADER_SIZE sizeof(uHeader)


	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::IO;
	using namespace System::Reflection;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(array<String^>^ arguments)
		{
			args = arguments;

			InitializeComponent();
			// Double Buffered Grid View please
			dataGridView->GetType()->GetProperty("DoubleBuffered", BindingFlags::Instance | BindingFlags::NonPublic)->SetValue(dataGridView, true);
			toolStripStatus->Text = "";
		}

	protected:

		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::Windows::Forms::MenuStrip^ menuStrip;
		System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^ saveAsToolStripMenuItem;
		System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem1;
		System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
		System::Windows::Forms::Panel^ panel1;
		System::Windows::Forms::DataGridView^ dataGridView;
		System::Windows::Forms::StatusStrip^ statusStrip;
		System::Windows::Forms::ToolStripStatusLabel^ toolStripStatus;
		System::Windows::Forms::OpenFileDialog^ openFileDialog;
		System::Windows::Forms::SaveFileDialog^ saveFileDialog;
		System::Windows::Forms::ToolStripMenuItem^ helpToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^ aboutToolStripMenuItem;
		System::Windows::Forms::DataGridViewTextBoxColumn^ n;
		System::Windows::Forms::DataGridViewTextBoxColumn^ assetStrings;
		System::Windows::Forms::DataGridViewTextBoxColumn^ assetValues;

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->dataGridView = (gcnew System::Windows::Forms::DataGridView());
			this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->n = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->assetStrings = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->assetValues = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->menuStrip->SuspendLayout();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView))->BeginInit();
			this->statusStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->helpToolStripMenuItem
			});
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(737, 24);
			this->menuStrip->TabIndex = 0;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->openToolStripMenuItem,
					this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->toolStripMenuItem1, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->openToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->openToolStripMenuItem->Text = L"&Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Enabled = false;
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
			this->saveToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->saveToolStripMenuItem->Text = L"&Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveToolStripMenuItem_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Enabled = false;
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift)
				| System::Windows::Forms::Keys::S));
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->saveAsToolStripMenuItem->Text = L"Save &As...";
			this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(192, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(195, 22);
			this->exitToolStripMenuItem->Text = L"E&xit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"&Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(107, 22);
			this->aboutToolStripMenuItem->Text = L"&About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::aboutToolStripMenuItem_Click);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::Control;
			this->panel1->Controls->Add(this->dataGridView);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(0, 24);
			this->panel1->Name = L"panel1";
			this->panel1->Padding = System::Windows::Forms::Padding(7, 0, 7, 0);
			this->panel1->Size = System::Drawing::Size(737, 389);
			this->panel1->TabIndex = 1;
			// 
			// dataGridView
			// 
			this->dataGridView->AllowUserToAddRows = false;
			this->dataGridView->AllowUserToResizeRows = false;
			this->dataGridView->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView->ColumnHeadersVisible = false;
			this->dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->n, this->assetStrings,
					this->assetValues
			});
			this->dataGridView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView->Location = System::Drawing::Point(7, 0);
			this->dataGridView->Name = L"dataGridView";
			this->dataGridView->RowHeadersVisible = false;
			this->dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
			this->dataGridView->Size = System::Drawing::Size(723, 389);
			this->dataGridView->TabIndex = 2;
			this->dataGridView->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dataGridView_CellClick);
			this->dataGridView->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dataGridView_CellValueChanged);
			this->dataGridView->SelectionChanged += gcnew System::EventHandler(this, &MainForm::dataGridView_SelectionChanged);
			// 
			// statusStrip
			// 
			this->statusStrip->BackColor = System::Drawing::SystemColors::Control;
			this->statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripStatus });
			this->statusStrip->Location = System::Drawing::Point(0, 413);
			this->statusStrip->Name = L"statusStrip";
			this->statusStrip->Size = System::Drawing::Size(737, 22);
			this->statusStrip->TabIndex = 2;
			this->statusStrip->Text = L"statusStrip";
			// 
			// toolStripStatus
			// 
			this->toolStripStatus->BackColor = System::Drawing::SystemColors::Control;
			this->toolStripStatus->Name = L"toolStripStatus";
			this->toolStripStatus->Padding = System::Windows::Forms::Padding(7, 0, 0, 0);
			this->toolStripStatus->Size = System::Drawing::Size(46, 17);
			this->toolStripStatus->Text = L"Status";
			// 
			// openFileDialog
			// 
			this->openFileDialog->Filter = L"UAsset File (*.uasset)|*.uasset";
			// 
			// saveFileDialog
			// 
			this->saveFileDialog->Filter = L"UAsset File (*.uasset)|*.uasset";
			// 
			// n
			// 
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleRight;
			this->n->DefaultCellStyle = dataGridViewCellStyle1;
			this->n->HeaderText = L"#";
			this->n->Name = L"n";
			this->n->ReadOnly = true;
			this->n->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->n->Width = 30;
			// 
			// assetStrings
			// 
			this->assetStrings->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->assetStrings->HeaderText = L"Strings";
			this->assetStrings->Name = L"assetStrings";
			this->assetStrings->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// assetValues
			// 
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleRight;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			dataGridViewCellStyle2->NullValue = nullptr;
			this->assetValues->DefaultCellStyle = dataGridViewCellStyle2;
			this->assetValues->HeaderText = L"Values";
			this->assetValues->Name = L"assetValues";
			this->assetValues->ReadOnly = true;
			this->assetValues->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// MainForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDark;
			this->ClientSize = System::Drawing::Size(737, 435);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip);
			this->Controls->Add(this->statusStrip);
			this->MainMenuStrip = this->menuStrip;
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->Text = L"UAssetEditor";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->Shown += gcnew System::EventHandler(this, &MainForm::MainForm_Shown);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &MainForm::MainForm_DragDrop);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &MainForm::MainForm_DragEnter);
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->panel1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView))->EndInit();
			this->statusStrip->ResumeLayout(false);
			this->statusStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	public:
		array<String^>^ args;

	private:
		String^ title = "UAssetEditor";
		String^ fileName;
		bool unsavedChanges = false;
		uHeader* header;
		char* strings;
		char* footer;
		int stringsSize;
		int footerSize;

		void openFile(String^ filename);
		bool saveFile(String^ filename);
		char* to_pchar(String^ s);

		System::Void MainForm_Shown(System::Object^ sender, System::EventArgs^ e)
		{
			if (args != nullptr && args->Length == 1 && args[0]->EndsWith(".uasset", StringComparison::CurrentCultureIgnoreCase))
			{
				openFile(args[0]);
			}
		}

		System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (unsavedChanges)
			{
				if (MessageBox::Show("There are unsaved changes. Are you sure you want to exit?", "Exit",
					MessageBoxButtons::YesNo, MessageBoxIcon::Warning, MessageBoxDefaultButton::Button2) == System::Windows::Forms::DialogResult::No)
				{
					e->Cancel = true;
				}
			}
		}

		System::Void MainForm_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			e->Effect = DragDropEffects::None;
			if (e->Data->GetDataPresent(DataFormats::FileDrop))
			{
				array<String^>^ files = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
				if (files != nullptr && files->Length == 1 && files[0]->EndsWith(".uasset", StringComparison::CurrentCultureIgnoreCase))
				{
					e->Effect = DragDropEffects::Move;
				}
			}
		}

		System::Void MainForm_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (e->Data->GetDataPresent(DataFormats::FileDrop))
			{
				array<String^>^ files = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
				if (files != nullptr && files->Length == 1 && files[0]->EndsWith(".uasset", StringComparison::CurrentCultureIgnoreCase))
				{
					toolStripStatus->Text = "";
					if (unsavedChanges)
					{
						if (MessageBox::Show("There are unsaved changes. Are you sure you want to open another file?", "Open",
							MessageBoxButtons::YesNo, MessageBoxIcon::Warning, MessageBoxDefaultButton::Button2) == System::Windows::Forms::DialogResult::No)
						{
							return;
						}
						unsavedChanges = false;
					}

					openFile(files[0]);
				}
			}
		}

		System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			toolStripStatus->Text = "";
			if (unsavedChanges)
			{
				if (MessageBox::Show("There are unsaved changes. Are you sure you want to open another file?", "Open",
					MessageBoxButtons::YesNo, MessageBoxIcon::Warning, MessageBoxDefaultButton::Button2) == System::Windows::Forms::DialogResult::No)
				{
					return;
				}
				unsavedChanges = false;
			}

			if (openFileDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

			openFile(openFileDialog->FileName);
		}
		System::Void saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (!unsavedChanges) return;
			if (saveFile(fileName))
			{
				unsavedChanges = false;
				openFile(fileName);

				toolStripStatus->Text = "File saved";
			}
		}

		System::Void saveAsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (saveFileDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

			if (saveFile(saveFileDialog->FileName))
			{
				unsavedChanges = false;
				openFile(saveFileDialog->FileName);

				toolStripStatus->Text = "File saved";
			}

		}

		System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			Close();
		}

		System::Void dataGridView_SelectionChanged(System::Object^ sender, System::EventArgs^ e)
		{
			dataGridView->ClearSelection();
		}

		System::Void dataGridView_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e)
		{
			dataGridView->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Selected = true;
			dataGridView->BeginEdit(true);
		}

		System::Void dataGridView_CellValueChanged(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e)
		{
			if (e->ColumnIndex != 1) return;
			if (System::String::IsNullOrEmpty(fileName)) return;
			if (!unsavedChanges)
			{
				Text = Text + "*";
				unsavedChanges = true;
			}

			saveToolStripMenuItem->Enabled = true;
		}

		System::Void aboutToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			MessageBox::Show("UAssetEditor version 1.0.2 by SamsamTS.", "About", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
	};
}
