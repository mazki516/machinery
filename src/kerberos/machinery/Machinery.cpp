
#include "machinery/Machinery.h"

namespace kerberos
{
    void Machinery::setup(const kerberos::StringMap & settings)
    {
        // -----------------------------------------------------------
        // Creates condition, algorithms, expositors, heuristics and io handlers.
        
        std::vector<Condition *> conditions = Factory<Condition>::getInstance()->createMultiple(settings.at("condition"));
        for(int i = 0; i < conditions.size(); i++)
        {
            conditions[i]->setup(settings);
        }
        setCondition(conditions);
        
        Algorithm * algorithm = Factory<Algorithm>::getInstance()->create(settings.at("algorithm"));
        algorithm->setup(settings);
        setAlgorithm(algorithm);

        Expositor * expositor = Factory<Expositor>::getInstance()->create(settings.at("expositor"));
        expositor->setup(settings);
        setExpositor(expositor);

        Heuristic * heuristic = Factory<Heuristic>::getInstance()->create(settings.at("heuristic"));
        heuristic->setup(settings);
        setHeuristic(heuristic);  

        std::vector<Io *> ios = Factory<Io>::getInstance()->createMultiple(settings.at("io"));
        for(int i = 0; i < ios.size(); i++)
        {
            ios[i]->setup(settings);
        }
        setIo(ios);
    }

    void Machinery::initialize(ImageVector & images)
    {
        m_algorithm->initialize(images);
        for(int i = 0; i < m_ios.size(); i++)
        {
            m_ios[i]->save(*images[images.size()-1]);
        }
    }
    
    void update(const ImageVector & images)
    {
        
    }

    bool Machinery::allowed(const ImageVector & images)
    {
        bool allowed = true;
        
        int i = 0;
        while(allowed && i < m_conditions.size())
        {
            allowed = m_conditions[i]->allowed(images);
            i++;
        }
        
        return allowed;
    }
    
    bool Machinery::save(Image & image, JSON & data)
    {
        bool success = true;
        
        int i = 0;
        while(success && i < m_ios.size())
        {
            success = m_ios[i]->save(image, data);
            i++;
        }
        
        return success;
    }

    bool Machinery::detect(ImageVector & images, JSON & data)
    {
        // -------------
        // Detect motion

        return detectMotion(images, data);
    }

    bool Machinery::detectMotion(ImageVector & images, JSON & data)
    {
        if(m_algorithm && m_expositor && m_heuristic)
        {
            Image evaluation = m_algorithm->evaluate(images, data);
            m_expositor->calculate(evaluation, data);
            return m_heuristic->isValid(evaluation, images, data);
        }
        return false;
    }
}
