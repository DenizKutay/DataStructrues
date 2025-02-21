#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    std::vector<std::pair<int,int>> edges;
    ImageSharpening imageSharpening;
    ImageMatrix sharp = imageSharpening.sharpen(img,2);
    EdgeDetector edgeDetector;
    edges = edgeDetector.detectEdges(sharp);
    DecodeMessage decodeMessage;
    std::string result = decodeMessage.decodeFromImage(sharp,edges);
    return result;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    std::vector<std::pair<int,int>> edges;
    ImageSharpening imageSharpening;
    ImageMatrix sharpImg = imageSharpening.sharpen(img,2);
    EdgeDetector edgeDetector;
    edges = edgeDetector.detectEdges(sharpImg);
    EncodeMessage encodeMessage;
    ImageMatrix encoded = encodeMessage.encodeMessageToImage(img,message,edges);

    return encoded;
}
