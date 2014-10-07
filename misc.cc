#include <string>
#include <vector>
#include <tinyxml2.h>
#include <cstdlib>

#include "misc.hh"
#include "camera.hh"

void parser_xml_to_camera(std::vector<Camera> *cameras)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("camerasInfo.xml");
    
    tinyxml2::XMLElement* xml_camera = doc.FirstChildElement("Camera");

    while (xml_camera != NULL)
    {
        std::string id(xml_camera->FirstChildElement("ID")->GetText());
        std::string width(xml_camera->FirstChildElement("Width")->GetText());
        std::string height(xml_camera->FirstChildElement("Height")->GetText());
        std::string expected_number_of_markers(xml_camera->FirstChildElement("ExpectedMarkers")->GetText());

        Camera camera(atoi(id.c_str()), atoi(width.c_str()),
                atoi(height.c_str()), atoi(expected_number_of_markers.c_str()));
        cameras->push_back(camera);

        xml_camera = xml_camera->NextSiblingElement("Camera");
    }
}
