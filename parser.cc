#include <string>
#include <vector>
#include <tinyxml2.h>
#include <cstdlib>

#include "misc.hh"
#include "camera.hh"


void parser_xml_to_cameras(std::vector<Camera> *cameras)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("camerasInfo.xml");
    
    tinyxml2::XMLElement* xml_camera = doc.FirstChildElement("Camera");

    while (xml_camera != NULL)
    {
        std::string id(xml_camera->FirstChildElement("ID")->GetText());
        std::string width(xml_camera->FirstChildElement("Width")->GetText());
        std::string height(xml_camera->FirstChildElement("Height")->GetText());
        std::string wc_height(xml_camera->FirstChildElement("WCHeight")->GetText());
        std::string wc_offset_x(xml_camera->FirstChildElement("WCOffX")->GetText());
        std::string wc_offset_y(xml_camera->FirstChildElement("WCOffY")->GetText());
        std::string wc_offset_angle(xml_camera->FirstChildElement("WCOffAngle")->GetText());

        Camera camera(atoi(id.c_str()), atoi(width.c_str()),
                atoi(height.c_str()), atof(wc_height.c_str()), atof(wc_offset_x.c_str()), 
                atof(wc_offset_y.c_str()), atof(wc_offset_angle.c_str()));
        cameras->push_back(camera);

        xml_camera = xml_camera->NextSiblingElement("Camera");
    }
}
