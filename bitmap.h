#include <fstream>
struct bmpfileheader
{
    //File Header
    char ID[2]; // "BM"
    unsigned short File_size[2]; // Size of file in bytes
    unsigned short Reserved[2]; // Must be 0
    unsigned short Offset[2]; // Offset to start of image data
    //Info Header
    unsigned short Info_Header_size[2]; // Size of Info header in bytes
    unsigned short Width[2]; // Width of image in pixels
    unsigned short Height[2]; // Height of image in pixels
    unsigned short Planes;
    unsigned short Bit_count; // Bits per pixel
    unsigned short Compression[2]; // Compression type
    unsigned short Image_size[2];
    unsigned short X_Res[2];
    unsigned short Y_Res[2];
    unsigned short Colors[2];
    unsigned short Important_Colors[2];
};

class Bmp_File
{
    public:
        Bmp_File(const char *file_name);
        ~Bmp_File();
        void RB_Flip();
        bool Error;
        bmpfileheader bmpheader;
        unsigned int File_size;
        unsigned int Offset;
        unsigned int Info_Header_size;
        unsigned int Width;
        unsigned int Height;
        unsigned short Planes;
        unsigned short Bit_count;
        unsigned int Compression;
        unsigned int Image_size;
        unsigned int X_Res;
        unsigned int Y_Res;
        unsigned int Colors;
        unsigned int Important_Colors;
        unsigned char *Data;
};

Bmp_File::Bmp_File(const char *file_name)
{
    Error = false;
    std::ifstream BmpIn(file_name, std::ios::binary);
    if (!BmpIn.is_open()) // If program cannot find file
    {
        Error = true;
    } else {
        BmpIn.read(reinterpret_cast<char *>(&bmpheader), 
                   sizeof(bmpheader));
        File_size = bmpheader.File_size[1] * 65536 + bmpheader.File_size[0];
        Offset = bmpheader.Offset[1] * 65536 + bmpheader.Offset[0];
        Info_Header_size = bmpheader.Info_Header_size[1] * 65536 + bmpheader.Info_Header_size[0];
        Width = bmpheader.Width[1] * 65536 + bmpheader.Width[0];
        Height = bmpheader.Height[1] * 65536 + bmpheader.Height[0];
        Planes = bmpheader.Planes;
        Bit_count = bmpheader.Bit_count;
        Compression = bmpheader.Compression[1] * 65536 + bmpheader.Compression[0];
        Image_size = bmpheader.Image_size[1] * 65536 + bmpheader.Image_size[0];
        X_Res = bmpheader.X_Res[1] * 65536 + bmpheader.X_Res[0];
        Y_Res = bmpheader.Y_Res[1] * 65536 + bmpheader.Y_Res[0];
        Colors = bmpheader.Colors[1] * 65536 + bmpheader.Colors[0];
        Important_Colors = bmpheader.Important_Colors[1] * 65536 + bmpheader.Important_Colors[0];
        Data = new unsigned char[Image_size];
        BmpIn.seekg(Offset, std::ios::beg);
        BmpIn.read(reinterpret_cast<char *>(Data), Image_size);
    }
    BmpIn.close();
    RB_Flip();
}

Bmp_File::~Bmp_File()
{
    delete[] Data;
}

void Bmp_File::RB_Flip()
{
    unsigned char temp;
    int x = 0;
    while (x < (Width * Height))
    {
        temp = Data[x * 3];
        Data[x * 3] = Data[(x * 3) + 2];
        Data[(x * 3) + 2] = temp;
        x++;
    }
}
