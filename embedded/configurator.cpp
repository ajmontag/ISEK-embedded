#include "configurator.h"

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>
#include <libxml++/attribute.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/element.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

Configurator::Configurator (const char * path)
{
    Glib::ustring name;
    const char *val;
    strcpy(path_, path);
    
    setDefaults();
    try
    {
        xmlpp::TextReader reader(path_);

        while(reader.read())
        {
            name = reader.get_name();
            if(reader.has_attributes())
            {
                reader.move_to_first_attribute();
                
                //val = reader.get_value().c_str();
                std::string str = reader.get_value().raw();
                val = str.c_str();
                std::cout << name << ": " << reader.get_value() << " --str-- " << str << " --cstr-- " << val << std::endl;
                int d = atoi(val);
                double f = atof(val);
                if(!name.compare("invertX"))
                    invertX_ = d;
                else if(!name.compare("invertY"))
                    invertY_ = d;
                else if(!name.compare("imageWidth"))
                    imageWidth_ = d;
                else if(!name.compare("imageHeight"))
                    imageHeight_ = d;
                else if(!name.compare("showGUI"))
                    showGUI_ = d;
                else if(!name.compare("deadzone"))
                    deadzone_ = f;
                else if(!name.compare("maxY"))
                    maxY_ = d;
                else if(!name.compare("minY"))
                    minY_ = d;
                else if(!name.compare("maxX"))
                    maxX_ = d;
                else if(!name.compare("minX"))
                    minX_ = d;
                else if(!name.compare("servo1"))
                    servo1_ = d;
                else if(!name.compare("servo2"))
                    servo2_ = d;
                else if(!name.compare("serialPortPath"))
                    strncpy(serialportpath_, val, 256);
                else if(!name.compare("nofacethreshold"))
                    nofacethreshold_ = d;
                else if(!name.compare("nofacefreamecount"))
                    nofacefreamecount_ = d;
                else if(!name.compare("CAM"))
                    CAM_ = d;
                else if(!name.compare("haarcascade"))
                    strncpy(haarcascade_, val, 256);
            }

        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

void Configurator::setDefaults()
{
    invertX_ = 1;
    invertY_ = 0;
    imageWidth_ = 160;
    imageHeight_ = 120;
    showGUI_ = 1;
    deadzone_ = 0.2;
    maxY_ = 180;
    minY_ = 0;
    maxX_ = 180;
    minX_ = 0;
    servo1_ = 9;
    servo2_ = 10;
    strncpy(serialportpath_, "/dev/ttyUSB0", 256);
    nofacefreamecount_ = 0;
    nofacethreshold_ = 0;
    CAM_ = 0;
    strncpy(haarcascade_, "/home/hermes/OpenCV-2.3.1/data/haarcascades/haarcascade_frontalface_alt.xml", 256);
}

void Configurator::saveLimits(int maxY, int minY, int maxX, int minX)
{
    Glib::ustring name;
    Glib::ustring empty;
    maxY_ = maxY;
    minY_ = minY;
    maxX_ = maxX;
    minX_ = minX;

    char bufMaxY[4];
    char bufMinY[4];
    char bufMaxX[4];
    char bufMinX[4];

    sprintf(bufMaxY, "%d", maxY);
    sprintf(bufMinY, "%d", minY);
    sprintf(bufMaxX, "%d", maxX);
    sprintf(bufMinX, "%d", minX);

    Glib::ustring sMaxY(bufMaxY);
    Glib::ustring sMinY(bufMinY);
    Glib::ustring sMaxX(bufMaxX);
    Glib::ustring sMinX(bufMinX);

    // Parse document to find the attribute to update
    
    try
    {
        xmlpp::DomParser parser;
    
        parser.parse_file(path_);
        xmlpp::Node* root = parser.get_document()->get_root_node();
        xmlpp::Node::NodeList list = root->get_children();
    
        for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(*iter))
            {
                const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
                for(xmlpp::Element::AttributeList::const_iterator iter2 = attributes.begin(); iter2 != attributes.end(); ++iter2)
                {
                    xmlpp::Attribute* attr = *iter2;
                    name = nodeElement->get_name();
                
                    if(!name.compare("maxY"))
                    {
                        attr->set_value(sMaxY);
                        std::cout << "maxY";
                    }
                    if(!name.compare("minY"))
                        attr->set_value(sMinY);
                    if(!name.compare("maxX"))
                        attr->set_value(sMaxX);
                    if(!name.compare("minX"))
                        attr->set_value(sMinX);
                }
            }
        }
        
        parser.get_document()->write_to_file(path_);
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return;
    }

    return;

}
