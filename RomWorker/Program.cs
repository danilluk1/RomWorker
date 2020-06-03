using System;
using System.IO;
using System.Linq;
using System.Text;

namespace RomFileWriter {

    /// <summary>
    /// Класс, записывающий данные в образ.
    /// </summary>
    public sealed class ClassROM {
        private const string RomSeparator = "||";
        private const long StorageMaxLengthBaits = 4294967296; //4 GB
        private const int FilenameMaxLength = 65535;

        private double _currentStorageLengthBaits;
        private string _romBaseDirectory;
        private string _romFileDirectory;

        public string RomBaseDirectory {
            get => _romBaseDirectory;
            set {
                if (value != null) {
                    if (Directory.Exists(value)) {
                        _romBaseDirectory = value;
                    }
                    else {
                        throw new ArgumentException("Указанной директории не существует.");
                    }
                }
                else {
                    throw new ArgumentException("Путь к базовой директории ROM не может быть пустым.");
                }
            }
        }
        public string RomFileDirectory {
            get => _romFileDirectory;
            set {
                if (value != null) {
                    if (File.Exists(value)) {
                        _romFileDirectory = value;
                    }
                    else {
                        throw new ArgumentException("Файл должен существовать.");
                    }
                }
                else {
                    throw new ArgumentException("Аргумент не может быть null.");
                }
            }
        }
        public ClassROM() { }

        public void Make() {
            var dirInfo = new DirectoryInfo(_romBaseDirectory).GetFiles().OrderByDescending(f => f.Length);
            foreach (FileInfo currentFile in dirInfo) {
                if (IsFileAbleToBeInTheRom(currentFile)) {
                    if (IsROMHasFreeSpaceToFile(currentFile)) {
                        AddFileToRom(currentFile);
                        AddStorageCurrentLength(currentFile);
                        Console.WriteLine("Занято хранилища " + _currentStorageLengthBaits + " / " + StorageMaxLengthBaits + "Байт.");
                    }
                }
            }
        }

        private string CreateRomStringForFile(FileInfo info) {
            return $"{Environment.NewLine}ROM:{info.Name.Replace("|", "_")}{RomSeparator}{info.Length}{Environment.NewLine}";
        }

        private long CountLengthOfFileInRom(FileInfo info) {
            return info.Length + Encoding.ASCII.GetByteCount(CreateRomStringForFile(info));
        }
        private bool IsROMHasFreeSpaceToFile(FileInfo info) {
            return _currentStorageLengthBaits + CountLengthOfFileInRom(info) <= StorageMaxLengthBaits;
        }
        private bool IsFileAbleToBeInTheRom(FileInfo info) {
            return (info.Name.Length <= FilenameMaxLength)
                    && (CountLengthOfFileInRom(info) <= StorageMaxLengthBaits);
        }

        private void WriteDataToRomFromFile(string path) {
            using (FileStream fs = File.Open(path, FileMode.Open, FileAccess.Read, FileShare.Read))
            using (BufferedStream bs = new BufferedStream(fs))
            using (BinaryReader br = new BinaryReader(bs, Encoding.UTF32)) {
                var size = 4194304;

                var bufferSize = size;
                byte[] buffer = new byte[size];

                while (br.BaseStream.Position != br.BaseStream.Length) {
                    buffer = br.ReadBytes(bufferSize);
                    using (FileStream fs2 = File.Open(RomFileDirectory, FileMode.Append, FileAccess.Write, FileShare.Write)) {
                        using (BinaryWriter writer = new BinaryWriter(fs2, Encoding.UTF32)) {
                            writer.Write(buffer);
                            Console.WriteLine(path + " " + fs2.Position);
                        }

                    }
                }


            }

        }
        private void AddFileToRom(FileInfo info) {
            using (BinaryWriter writer = new BinaryWriter(File.Open(RomFileDirectory, FileMode.Append), Encoding.GetEncoding(1251))) {
                writer.Write(Encoding.Default.GetBytes(CreateRomStringForFile(info)));
            }
            WriteDataToRomFromFile(info.FullName);
        }
        private void AddStorageCurrentLength(FileInfo info) {
            _currentStorageLengthBaits += CountLengthOfFileInRom(info);
        }

    }

    class Program {
        static void Main() {
            var rom = new ClassROM {
                RomBaseDirectory = @"D:\ROM\RomBaseFolder\",
                RomFileDirectory = @"D:\ROM\eeprom.rom"
            };
            rom.Make();
            Console.ReadLine();
        }
    }
}
