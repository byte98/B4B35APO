#include <iostream>

#ifdef __unix__         
#include <unistd.h>
#include <fcntl.h>
#endif
#define DEBUG


#define compute_address(top, left, width) ((((top) * (width)) + (left)) * (3)) 
#define get_red(top, left, width) (((compute_address((top), (left), (width))) + (0)))
#define get_blue(top, left, width) (((compute_address((top), (left), (width))) + (1)))
#define get_green(top, left, width) (((compute_address((top), (left), (width))) + (2)))
#define OK 0

#define OUTPUT "./output.ppm"

#define good(value) ((value) == (OK))
int main(int argc, char *argv[])
{
#ifdef __unix__         
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef DEBUG
#ifdef __unix__  
	std::cout << "Compiled on Linux" << std::endl;
#elif defined(_WIN32) || defined(WIN32) 
	std::cout << "Compiled on Windows" << std::endl;
#endif
#endif // DEBUG


	int reti = OK;
	const int MASK[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};


	int WIDTH = 0, HEIGHT = 0;

	//Load dimensions from file
	FILE* fp;
#ifndef DEBUG
	fp = fopen(argv[1], "rb");
#endif // !DEBUG
#ifdef __unix__
	fp = fopen(argv[1], "rb");
#endif // __unix__

#ifdef DEBUG
#if defined(_WIN32) || defined(WIN32) 
	fp = fopen("./test.ppm", "rb");
#endif // defined(_WIN32) || defined(WIN32) 	
#endif // DEBUG


	reti = (fscanf(fp, "P6 \n %d \n %d \n 255 \n", &WIDTH, &HEIGHT) == 3);
	if (good(reti))
	{
#ifdef DEBUG
		std::cout << "Loaded dimensions: width=" << WIDTH << ", height=" << HEIGHT << std::endl;
#endif // DEBUG

		char** line_buffer = new char*[3];
		char** out_buffer = new char*[3];
		int loaded = 0, load_idx = 0, save_idx = 0, compute_index = 0;
		for (int i = 0; i < 3; i++)
		{
			line_buffer[i] = new char[WIDTH * 3];
			out_buffer[i] = new char[WIDTH * 3];
		}



		//Write PPM header
		FILE *out_temp;
		out_temp = fopen(OUTPUT, "w");
		fprintf(out_temp, "P6\n%d\n%d\n255\n", WIDTH, HEIGHT);
		fclose(out_temp);
#ifdef __unix__         
		int out = open(OUTPUT, O_WRONLY | O_APPEND);
#elif defined(_WIN32) || defined(WIN32) 
		FILE *out;
		out = fopen(OUTPUT, "ab");
#endif


		while (load_idx < HEIGHT)
		{
			//Read line
			fread(line_buffer[load_idx % 3], sizeof(char), WIDTH * 3, fp);
			load_idx++;

			if (load_idx >= 2)
			{
				if (compute_index == 0 || compute_index == HEIGHT - 1)
				{
					out_buffer[compute_index % 3] = line_buffer[(load_idx - 2) % 3];
					compute_index++;
				}
				else
				{
					for (int i = 0; i < WIDTH; i++)
					{
						if (i == 0)
						{

						}
						else
						{
							char R, G, B;
							R = (line_buffer[(load_idx - 2) % 3][((i - 1) * 3) + 0] * MASK[0][0]) + (line_buffer[(load_idx - 2) % 3][(i * 3) + 0] * MASK[0][1]) + (line_buffer[(load_idx - 2) % 3][((i + 1) * 3) + 0] * MASK[0][2]) +
								(line_buffer[(load_idx - 1) % 3][((i - 1) * 3) + 0] * MASK[1][0]) + (line_buffer[(load_idx - 1) % 3][(i * 3) + 0] * MASK[1][1]) + (line_buffer[(load_idx - 1) % 3][((i + 1) * 3) + 0] * MASK[1][2]) +
								(line_buffer[(load_idx - 0) % 3][((i - 1) * 3) + 0] * MASK[2][0]) + (line_buffer[(load_idx - 0) % 3][(i * 3) + 0] * MASK[2][1]) + (line_buffer[(load_idx - 0) % 3][((i + 1) * 3) + 0] * MASK[2][2]);

							G = (line_buffer[(load_idx - 2) % 3][((i - 1) * 3) + 1] * MASK[0][0]) + (line_buffer[(load_idx - 2) % 3][(i * 3) + 1] * MASK[0][1]) + (line_buffer[(load_idx - 2) % 3][((i + 1) * 3) + 1] * MASK[0][2]) +
								(line_buffer[(load_idx - 1) % 3][((i - 1) * 3) + 1] * MASK[1][0]) + (line_buffer[(load_idx - 1) % 3][(i * 3) + 1] * MASK[1][1]) + (line_buffer[(load_idx - 1) % 3][((i + 1) * 3) + 1] * MASK[1][2]) +
								(line_buffer[(load_idx - 0) % 3][((i - 1) * 3) + 1] * MASK[2][0]) + (line_buffer[(load_idx - 0) % 3][(i * 3) + 1] * MASK[2][1]) + (line_buffer[(load_idx - 0) % 3][((i + 1) * 3) + 1] * MASK[2][2]);

							B = (line_buffer[(load_idx - 2) % 3][((i - 1) * 3) + 2] * MASK[0][0]) + (line_buffer[(load_idx - 2) % 3][(i * 3) + 2] * MASK[0][1]) + (line_buffer[(load_idx - 2) % 3][((i + 1) * 3) + 2] * MASK[0][2]) +
								(line_buffer[(load_idx - 1) % 3][((i - 1) * 3) + 2] * MASK[1][0]) + (line_buffer[(load_idx - 1) % 3][(i * 3) + 2] * MASK[1][1]) + (line_buffer[(load_idx - 1) % 3][((i + 1) * 3) + 2] * MASK[1][2]) +
								(line_buffer[(load_idx - 0) % 3][((i - 1) * 3) + 2] * MASK[2][0]) + (line_buffer[(load_idx - 0) % 3][(i * 3) + 2] * MASK[2][1]) + (line_buffer[(load_idx - 0) % 3][((i + 1) * 3) + 2] * MASK[2][2]);

							out_buffer[compute_index % 3][(i * 3) + 0] = R;
							out_buffer[compute_index % 3][(i * 3) + 1] = G;
							out_buffer[compute_index % 3][(i * 3) + 2] = B;
						}
					}
				}
				//Write line
#ifdef __unix__         
				write(out, out_buffer[save_idx % 3], WIDTH * 3);
#elif defined(_WIN32) || defined(WIN32) 
				fwrite(out_buffer[save_idx % 3], sizeof(char), WIDTH * 3, out);
#endif
			}

		}
		while (save_idx < HEIGHT)
		{
			//Write line
			save_idx++;
#ifdef __unix__         
			write(out, out_buffer[save_idx % 3], WIDTH * 3);
#elif defined(_WIN32) || defined(WIN32) 
			fwrite(out_buffer[save_idx % 3], sizeof(char), WIDTH * 3, out);
#endif
		}


#ifdef __unix__         
		close(out);
#elif defined(_WIN32) || defined(WIN32) 
		fclose(out);
#endif
		

		delete[] line_buffer;
	}
	fclose(fp);


	return reti;
}