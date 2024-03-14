#include <cam_dif/Motioncapture.hpp>

void MotionCapture::detectionF(sensor_msgs::msg::Image::SharedPtr &ImageOutput, const sensor_msgs::msg::Image::ConstSharedPtr &ImageInput)
{
    auto width = ImageInput->width;
    auto height = ImageInput->height;
    auto step = ImageInput->step;
    auto data = ImageInput->data;

    // height: Die Höhe des Bildes, also die Anzahl der Zeilen.

    // width: Die Breite des Bildes, also die Anzahl der Spalten.

    // encoding: Der Encoding-Typ der Pixel, einschließlich der Bedeutung der Kanäle, der Reihenfolge und der Größe. Die gültigen Werte für das Encoding sind in der Datei src/image_encodings.cpp definiert 1.

    // is_bigendian: Gibt an, ob die Daten in Big-Endian-Reihenfolge sind.

    // step: Die vollständige Zeilenlänge in Bytes.

    // data: Die eigentlichen Matrixdaten. Die Größe ist (step * rows

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
            avg = (avg < 255 ? 255 : 0);
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

            // wrong stuff end
        }

        // old variation
        // for (uint32_t y = 0; y < height; y++)
        // {
        //     for (uint32_t x = 0; x < step; x++)
        //     {
        //         auto base = x + y * step;
        //         auto a = data[base];
        //         auto b = prev_data[base];

        //         data[base] = a - b;
        //         prev_data[base] = a;
        //     }
        // }
        // End old variation

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