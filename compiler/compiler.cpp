#include "compiler.h"
#include "slz/slz.h"
#include <dirent.h>
#include <sys/stat.h>

#define FIX32_INT_BITS  22
#define FIX32_FRAC_BITS (32 - FIX32_INT_BITS)
#define FIX32(value)    ((int) ((value) * (1 << FIX32_FRAC_BITS)))

template<class T>
std::string t_to_string(T i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
}

FILE* data_file_h;
FILE* data_file_c;

void compile_bitmap(string root_path, string bitmap_path)
{
    string bitmap_name = bitmap_path.substr(0, bitmap_path.size()-4);
    printf("compiling %s...\n", bitmap_name.c_str());

    FILE* bitmap = fopen((root_path+bitmap_path).c_str(), "rb");
    
    // bitmap header

    char bitmap_header[2];
    fread(bitmap_header, 1, 2, bitmap);
    unsigned int bitmap_size;
    fread(&bitmap_size, 4, 1, bitmap);
    unsigned int reserved;
    fread(&reserved, 4, 1, bitmap);
    unsigned int bitmap_offset;
    fread(&bitmap_offset, 4, 1, bitmap);
    
    // DIB header

    unsigned int dib_size;
    fread(&dib_size, 4, 1, bitmap);
    unsigned int bitmap_width;
    fread(&bitmap_width, 4, 1, bitmap);
    unsigned int bitmap_height;
    fread(&bitmap_height, 4, 1, bitmap);
    unsigned int reserved2;
    fread(&reserved2, 2, 1, bitmap);
    unsigned short bitmap_bpp;
    fread(&bitmap_bpp, 2, 1, bitmap);
    fread(&reserved2, 4, 1, bitmap);
    fread(&reserved2, 4, 1, bitmap);
    fread(&reserved2, 4, 1, bitmap);
    fread(&reserved2, 4, 1, bitmap);
    unsigned int bitmap_depth;
    fread(&bitmap_depth, 4, 1, bitmap);
    
    if (bitmap_depth==0)
        bitmap_depth = 16;
    if (bitmap_depth<3 || bitmap_depth>16 || bitmap_bpp!=4)
    {
        printf("INCORRECT DEPTH\n");
        fclose(bitmap);
        return;
    }

    int packed_width = (bitmap_width*bitmap_bpp)/16;
    int row_size = bitmap_width*bitmap_bpp/8;
    if (bitmap_width!=(packed_width*16)/bitmap_bpp)
    {
        printf("INCORRECT SIZE: %d,%d (%d,%f)\n", bitmap_width, (packed_width*16)/bitmap_bpp, bitmap_bpp, (bitmap_width*bitmap_bpp)/16.0f);
        fclose(bitmap);
        return;
    }
    
    bitmap_width = packed_width;

    fprintf(data_file_h, "    extern smeBitmap %s;\n", bitmap_name.c_str());
    
    FILE* packed_input = fopen((root_path+"tmp.in").c_str(), "wb");
    {
        // palette

        fseek(bitmap, dib_size+14, SEEK_SET);
        
        for (unsigned int c=0 ; c<bitmap_depth ; ++c)
        {
            unsigned char b;
            fread(&b, 1, 1, bitmap);
            unsigned char g;
            fread(&g, 1, 1, bitmap);
            unsigned char r;
            fread(&r, 1, 1, bitmap);
            unsigned char a;
            fread(&a, 1, 1, bitmap);
        
            unsigned short v = ((r*15/255)<<8)+((g*15/255)<<12)+((b*15/255)<<0);
            fwrite(&v, 2, 1, packed_input);
        }

        // read image data

        unsigned short* image = (unsigned short*)malloc(bitmap_width*bitmap_height*2);

        for (unsigned int y=0 ; y<bitmap_height ; ++y)
        {
            fseek(bitmap, bitmap_offset+(bitmap_height-y-1)*row_size, SEEK_SET);
            for (unsigned int x=0 ; x<bitmap_width ; ++x)
            {
                fread(image+y*bitmap_width+x, 2, 1, bitmap);
                ((unsigned char*)(image+y*bitmap_width+x))[0] += 1+(1<<4);
                ((unsigned char*)(image+y*bitmap_width+x))[1] += 1+(1<<4);
            }
        }

        fwrite(image, 2, bitmap_width*bitmap_height, packed_input);
        free(image);
    }
    fclose(packed_input);
    
    // compress

    packed_input = fopen((root_path+"tmp.in").c_str(), "rb");
    FILE* packed_output = fopen((root_path+"tmp.out").c_str(), "wb");
    compress(packed_input, packed_output, FORMAT_SLZ16);
    fclose(packed_output);
    fclose(packed_input);
    
    packed_output = fopen((root_path+"tmp.out").c_str(), "rb");
    fseek(packed_output, 0, SEEK_END);
    long size = ftell(packed_output);
    unsigned char* packed = (unsigned char*)malloc(size);
    fseek(packed_output, 0, SEEK_SET);
    fread(packed, 1, size, packed_output);
    fclose(packed_output);
    
    // write
    fprintf(data_file_c, "u8 %s_packed[] = {", bitmap_name.c_str());
    
    for (int i=0 ; i<size ; ++i)
    {
        fprintf(data_file_c, "%u", packed[i]);
        if (i<size-1)
            fprintf(data_file_c, ",");
    }
    free(packed);

    fprintf(data_file_c, "};\n");

    fprintf(data_file_c, "smeBitmap %s = {\n", bitmap_name.c_str());
    fprintf(data_file_c, "    %i,\n", bitmap_width);
    fprintf(data_file_c, "    %i,\n", bitmap_height);
    fprintf(data_file_c, "    %i,\n", bitmap_depth);
    fprintf(data_file_c, "    NULL,\n");
    fprintf(data_file_c, "    NULL,\n");
    fprintf(data_file_c, "    NULL,\n");
    fprintf(data_file_c, "    %s_packed\n", bitmap_name.c_str());
    fprintf(data_file_c, "};\n");
    fclose(bitmap);
}

int main(int argc, char* argv[])
{
    string root_path = argv[1];
    printf("compiling data...\n");

    data_file_h = fopen((root_path+"data.h").c_str(), "wt");
    fprintf(data_file_h, "#ifndef __DATA_H__\n");
    fprintf(data_file_h, "#define __DATA_H__\n\n");
    fprintf(data_file_h, "    #include \"sme/sme.h\"\n\n");

    data_file_c = fopen((root_path+"data.c").c_str(), "wt");
    fprintf(data_file_c, "#include \"data.h\"\n\n");

    {
	DIR *dp;
    	struct dirent *entry;
    	struct stat statbuf;
    	if((dp = opendir(root_path.c_str())) == NULL) {
        	return -1;
    	}
    	while((entry = readdir(dp)) != NULL) {
        	lstat(entry->d_name,&statbuf);
        	if(!S_ISDIR(statbuf.st_mode))
		{
			string ext = entry->d_name;
			if (ext.size()>4)
			{
				ext = ext.substr(ext.size()-3);
				if (ext=="bmp")
					compile_bitmap(root_path, entry->d_name);
			}
		}
    	}
    	closedir(dp);
    }

    fprintf(data_file_h, "\n#endif\n");
    fclose(data_file_h);
    return 0;
}

