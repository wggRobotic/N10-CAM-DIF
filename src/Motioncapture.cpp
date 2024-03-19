#include <cam_dif/Motioncapture.hpp>

void MotionCapture::detectionF(sensor_msgs::msg::Image::SharedPtr &ImageOutput, const sensor_msgs::msg::Image::ConstSharedPtr &ImageInput)
{
    auto width = ImageInput->width;
    auto height = ImageInput->height;
    auto step = ImageInput->step;
    auto data = ImageInput->data;



    if (prev_data.empty())
    {
        for (size_t i = 0; i < data.size(); i++)
            prev_data.push_back(data[i]);
    }
    else
    {
        size_t comp = step / width;
        for (size_t pixel = 0; pixel < data.size(); pixel += comp)
        {
            uchar avg = 0;
            // here is something wrong ;(
            for (int color = 0; color < comp; color++)
            {
                auto a = data[pixel + color];
                auto b = prev_data[pixel + color];
                auto c = a - b;

                avg += c;
                prev_data[pixel + color] = a;
            }
            avg = (avg < 255 ? 0 : 255);
            int num = 0;
            for (int color = 0; color < comp; color++)
            {

                
                switch (num)
                {
                case 0: //blue
                    data[pixel + color] = avg; 
                    break;
                case 1://green
                    data[pixel + color] = avg;
                    break;
                case 2:// red 
                    data[pixel + color] = avg;
                    num = -1;
                    break;
                default:
                    break;
                }
                ;
                num++;
            }


            
        }

        //cross morphing
        // for (uint32_t y = 1; y < height-1; y++)
        // {
        //     for (uint32_t x = 1; x < (width-1)*3; x++)
        //     {
                
        //         for (size_t i = 0; i < 3; i++)
        //         {
        //             int avgp = 0;
        //             avgp=(data[(x+1)+y*step+i]+data[(x+1)+(y+1)*step+i]+data[(x+1)+(y-1)*step+i]+data[(x-1)+y*step+i]+data[(x-1)+(y+1)*step+i]+data[(x-1)+(y+1)*step+i]+data[x+(y+1)*step+i]+data[x+(y-1)*step+i]+data[x+y*step+i])/9;
        //             data[x+y*step+i]= avgp;
        //         }
                
        //     }
        // }
        

        ImageOutput->width = width;
        ImageOutput->height = height;
        ImageOutput->step = step;
        ImageOutput->encoding = ImageInput->encoding;
        ImageOutput->header = ImageInput->header;
        ImageOutput->is_bigendian = ImageInput->is_bigendian;
        ImageOutput->data = data;
        ImageOutput->data = data;
    }
}

void MotionCapture::detectionI()
{
}