#include <png.h>
#include <getopt.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

#define RGB 3
#define ERR_INVALID_INPUT_ARGUMENTS 40
#define ERR_FILE_OPEN 41
#define ERR_NOT_PNG 42
#define ERR_PNG_STRUCT 43
#define ERR_COLOR_FORMAT 44
#define ERR_SPLIT_PARAMS 45
#define ERR_MEMORY 46
#define ERR_IMAGE_PROCESS 47
#define ERR_WRITE_FILE 48
#define ERR_INVALID_FILENAME 49


typedef struct PNG{
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;
}PNG;

typedef struct RGBAPixels{ // есть возможность добавления альфа канала, тогда надо будет подправить функцию replace_color и color_reading в первую очередь
    png_byte red;
    png_byte green;
    png_byte blue;
}RGBAPixels;

void info_for_image(PNG* image);
void print_help();
void read_image(const char* filename, PNG* image);
void write_for_file(const char* filename, PNG* image);
RGBAPixels color_reading(const char* color_rgb);
void replace_color(PNG* image, RGBAPixels old_color, RGBAPixels new_color);
void rgb_filter(PNG* image, const char* component_name, int component_value);
void lines_split(PNG* image, int num_x, int num_y, int thickness, RGBAPixels color);
void info_for_work();
void free_png(PNG* image);

int main(int argc, char* argv[]){
    if(argc < 2){
        print_help();
        return ERR_INVALID_INPUT_ARGUMENTS;
    }

    info_for_work();

    //какая операция должна быть выполнена
    bool replace = false;
    bool filter = false;
    bool split = false;
    bool inform = false;

    //изначальные данные 
    char* input_file = NULL;
    char* output_file_default = "out.png";
    RGBAPixels old_color = {0, 0, 0};
    RGBAPixels new_color = {0, 0, 0};

    char* component_name = NULL;
    int component_value = 0;

    int number_x = 2, number_y = 2, thickness = 1;
    RGBAPixels color_for_split = {0, 0, 0};


    int opt;
    while(true){

        static struct option long_options[] = {
            {"input", required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"help", no_argument, 0, 'h'},
            {"info", no_argument, 0, 1},
            {"color_replace", no_argument, 0, 2},
            {"old_color", required_argument, 0, 3},
            {"new_color", required_argument, 0, 4},
            {"rgbfilter", no_argument, 0, 5},
            {"component_name", required_argument, 0, 6},
            {"component_value", required_argument, 0, 7},
            {"split", no_argument, 0, 8},
            {"number_x", required_argument, 0, 9},
            {"number_y", required_argument, 0, 10},
            {"thickness", required_argument, 0, 11},
            {"color", required_argument, 0, 12},
            {0, 0, 0, 0}
         };

        opt = getopt_long (argc, argv, "hio:", long_options, NULL);

        if(opt == -1) break;

        switch(opt){
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file_default = optarg;
                break;
            case 'h':
                print_help();
                return 0;
            case 1:
                inform = true;
                break;
            case 2:
                replace = true;
                break;
            case 3:
                old_color = color_reading(optarg);
                break;
            case 4:
                new_color = color_reading(optarg);
                break;
            case 5:
                filter = true;
                break;
            case 6:
                component_name = optarg;
                break;
            case 7:
                component_value = atoi(optarg);
                break;
            case 8:
                split = true;
                break;
            case 9:
                number_x = atoi(optarg);
                break;
            case 10:
                number_y = atoi(optarg);
                break;
            case 11:
                thickness = atoi(optarg);
                break;
            case 12:
                color_for_split = color_reading(optarg);
                break;
            default:
                fprintf(stderr, "Error: Unknown option\n");
                print_help();
                exit(ERR_INVALID_INPUT_ARGUMENTS);
        }


    }

    //для получения имени файла
    if(input_file == NULL) {
       if(optind < argc) {
            input_file = argv[optind];
        } else {
            fprintf(stderr, "Error: No input file specified\n");
            print_help();
            return ERR_INVALID_FILENAME;
        }
    }

    //проверка, если пользователь хочет ввести несколько комманд 
    int count = 0;
    if(replace) count++;
    if(filter) count++;
    if(split) count++;
    if(count > 1){
        fprintf(stderr, "Error: one operation can be processed\n");
        print_help();
        return ERR_INVALID_INPUT_ARGUMENTS;

    }

    PNG image;
    read_image(input_file, &image);

    if(inform){
        info_for_image(&image);
        return 0;
    }

    if(replace){
        replace_color(&image, old_color, new_color);
    } else if(filter){
        rgb_filter(&image, component_name, component_value);
     } else if(split){
         lines_split(&image, number_x, number_y, thickness, color_for_split);
     }

    write_for_file(output_file_default, &image);
    
    void free_png(PNG* image);
    return 0;
}   

//функции чтения, заполнения файлов + вспомогательные 

void info_for_work(){
    puts("Course work for option 4.14, created by Khvedynich Varvara.");
}


void print_help(){
    puts("Use the following available options:");
    puts("  ");
    puts("-h|--help");
    puts("  ");
    puts("      utility information output");
    puts("  ");
    puts("-i|--info");
    puts("  ");
    puts("      output detailed information about the png file");
    puts("  ");
    puts("--color_replace");
    puts("  ");
    puts("      --old_color RRR.GGG.BBB    the colour to be replaced");
    puts("      --new_color RRR.GGG.BBB    the colour that's being used to replace");
    puts("  ");
    puts("--rgbfilter");
    puts("  ");
    puts("      --component_name blue|green|red    the colour to be replaced");
    puts("      --component_value 0-255    the colour that's being used to replace");
    puts("  ");
    puts("--split");
    puts("  ");
    puts("      --number_x X    number >= 1");
    puts("      --number_y Y    number >= 1");
    puts("      --thickness NUM    number >= 0");
    puts("      --color RRR.GGG.BBB    line colour");
    puts("  ");
}

void read_image(const char* filename, PNG* image){
    FILE* file = fopen(filename, "rb"); 
    if(!file){
        fprintf(stderr, "Error: File read error\n");
        exit(ERR_FILE_OPEN);
    }
    
    unsigned char header[8]; // проверка на пнг с чтением первых 8 байт 
    fread(header, 1, 8, file);
    if(png_sig_cmp(header, 0, 8)){
        puts("The file is not a png file. Input a png file");
        fprintf(stderr, "Error: Unknown option\n"); //
        fclose(file);
        exit(ERR_NOT_PNG);
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
    if(!image->png_ptr){
        exit(ERR_PNG_STRUCT);
        fclose(file);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr); 
    if(!image->info_ptr){
        png_destroy_read_struct(&image->png_ptr, NULL, NULL);
        fclose(file);
        exit(ERR_PNG_STRUCT);
    }
    
    if (setjmp(png_jmpbuf(image->png_ptr))) // обработка ошибок 
    {
        png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
        fclose(file);
        exit(ERR_PNG_STRUCT);
    }
    
    png_init_io(image->png_ptr, file); // читаем инфу об изобр и пр 
    png_set_sig_bytes(image->png_ptr, 8);
    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr); //заполнение инфы 
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    //всякие преобразования для удобства работы с картинкой 
    if (image->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(image->png_ptr);
    if (image->color_type == PNG_COLOR_TYPE_GRAY && image->bit_depth < 8)  
        png_set_expand_gray_1_2_4_to_8(image->png_ptr);
    if (png_get_valid(image->png_ptr, image->info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(image->png_ptr);
    if (image->bit_depth == 16)
        png_set_strip_16(image->png_ptr);
    if (image->color_type == PNG_COLOR_TYPE_GRAY 
        || image->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
          png_set_gray_to_rgb(image->png_ptr);
    
    png_read_update_info(image->png_ptr, image->info_ptr); 

    image->row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*image->height);
    for(int i = 0; i < image->height; i++){
        image->row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));
    }
    
    png_read_image(image->png_ptr, image->row_pointers);
    png_read_end(image->png_ptr, NULL);
    png_destroy_read_struct(&image->png_ptr, &image->info_ptr, NULL);
    fclose(file);

}

void write_for_file(const char* filename, PNG* image){
    FILE* file = fopen(filename, "wb");
    if(!file){
        fprintf(stderr, "Error: File write error\n"); 
        fclose(file);
        exit(ERR_WRITE_FILE);
    }

    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!image->png_ptr){
        exit(ERR_WRITE_FILE);
        fclose(file);
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if(!image->info_ptr){
        png_destroy_write_struct(&image->png_ptr, NULL);
        fclose(file);
        exit(ERR_PNG_STRUCT);
    }

    if (setjmp(png_jmpbuf(image->png_ptr))) // обработка ошибок 
    {
        png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
        fclose(file);
        exit(ERR_PNG_STRUCT);
    }

    png_init_io(image->png_ptr, file);

    png_set_IHDR(
        image->png_ptr, 
        image->info_ptr, 
        image->width, 
        image->height,
        image->bit_depth,
        image->color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE,
        PNG_FILTER_TYPE_BASE  
        );


    png_write_info(image->png_ptr, image->info_ptr);
    png_write_image(image->png_ptr, image->row_pointers);
    png_write_end(image->png_ptr, NULL);

    png_destroy_write_struct(&image->png_ptr, &image->info_ptr);
    fclose(file);

}

void info_for_image(PNG* image){
    printf("Height: %d pixel\n", image->height);
    printf("Width: %d pixel\n", image->width);
    printf("Color_type: %d\n", image->color_type);
    printf("Bit_depth: %d\n", image->bit_depth);
}

RGBAPixels color_reading(const char* color_rgb){ //вспомогательная функция
    RGBAPixels color = {0, 0, 0}; // альфа канал = 255 => непрозрачность
    unsigned int r, g, b;

    int count = sscanf(color_rgb, "%u.%u.%u", &r, &g, &b);

    if ( count < 3){
        puts("The colours are incorrectly rendered. Transmit in the format RRR.GGG.BBB");
        exit(ERR_INVALID_INPUT_ARGUMENTS);
    }

    if(r > 255 || g > 255 || b > 255){
        fprintf(stderr, "Error: The colour can have a value from 0 to 255\n"); //
        exit(ERR_COLOR_FORMAT);
    }

    color.red = (png_byte)r;
    color.green = (png_byte)g;
    color.blue = (png_byte)b;

    return color;
}

void free_png(PNG* image) {
    if(image->row_pointers) {
        for(int i = 0; i < image->height; i++) {
            free(image->row_pointers[i]);
        }
        free(image->row_pointers);
    }
}

//

//основные функции, нужные по заданию 

void replace_color(PNG* image, RGBAPixels old_color, RGBAPixels new_color){
    int channels;
    switch(image->color_type){
        case PNG_COLOR_TYPE_RGB:
            channels = 3;
            break;
        default:
            fprintf(stderr, "Error: Incorrect number of channels. The programme processes RGB images\n");
            exit(ERR_IMAGE_PROCESS);
    }
    
    for(int y = 0; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for(int x = 0; x < image->width; x++){
            png_byte *pixel = &(row[x*channels]);
            
            bool match = true;

            if(pixel[0] != old_color.red) match = false;
            if(pixel[1] != old_color.green) match = false;
            if(pixel[2] != old_color.blue) match = false;

            if ( match ){
                pixel[0] = new_color.red;
                pixel[1] = new_color.green;
                pixel[2] = new_color.blue;
            }

        }
    }

}

void rgb_filter(PNG* image, const char* component_name, int component_value){
    if(!component_name || component_value < 0 || component_value > 255){
        fprintf(stderr, "Error: The colour can have a value from 0 to 255\n");
        exit(ERR_INVALID_INPUT_ARGUMENTS);
    }


    int index = 0;
    if(strcmp(component_name, "green") == 0) index = 1;
    else if (strcmp(component_name, "blue") == 0) index = 2;

    for(int y = 0; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for(int x = 0; x < image->width; x++){
            png_byte *pixel = &(row[x*RGB]);

            pixel[index] = component_value;
        }
    }
}

void lines_split(PNG* image, int num_x, int num_y, int thickness, RGBAPixels color){
    if(num_x == 0 || num_y == 0){
        fprintf(stderr, "Error: The value must be greater than or equal to 1\n");
        exit(ERR_SPLIT_PARAMS);
    }

    if(thickness > image->width/2 || thickness > image->height/2){
        fprintf(stderr, "The line is too thick\n"); 
        exit(ERR_SPLIT_PARAMS);
    }

    int x_pix_square = image->width / num_x;
    int y_pix_square = image->height / num_y;
    //скок пикселей на квадрат без учета толщины линии

    for(int y = 0; y < image->height; y++){
        png_byte *row = image->row_pointers[y];
        for(int x = 0; x < image->width; x++){
            png_byte *pixel = &(row[x*RGB]);

            for(int i = 1; i < num_x; i++){
                if(x <= (i*x_pix_square + thickness/2) && x >= (i*x_pix_square - thickness/2)){
                    pixel[0] = color.red;
                    pixel[1] = color.green;
                    pixel[2] = color.blue;
                }
            }

            for(int j = 1; j < num_y; j++){
                if(y <= (j*y_pix_square + thickness/2) && y >= (j*y_pix_square - thickness/2)){
                    pixel[0] = color.red;
                    pixel[1] = color.green;
                    pixel[2] = color.blue;
                }

            }


        }
    }

}
