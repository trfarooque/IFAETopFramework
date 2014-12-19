#include "IFAETopFramework/SystManager.h"
#include "IFAETopFramework/OptionsBase.h"
#include "IFAETopFramework/NtupleData.h"

//________________________________________________________________________
//
SystManager::SystManager( OptionsBase *opt ):
m_systVector(0),
m_opt(opt)
{
    if(!m_systVector){
        m_systVector = new SystVector();
    }
}

//________________________________________________________________________
//
SystManager::~SystManager()
{}

//________________________________________________________________________
//
bool SystManager::AddToSystVector( const WeightSys::wgtSys &sys_type, const std::string &name) const
{
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in SystManager::AddToSystVector()" << std::endl;
    
    Systematic *sys = new Systematic();
    sys->sysType = sys_type;
    sys->name = name;
    sys->value = 0;
    m_systVector->push_back(sys);
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving SystManager::AddToSystVector()" << std::endl;
    
    return true;
}

//________________________________________________________________________
//
bool SystManager::LoadWeightSysts() const
{
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in SystManager::LoadWeightSysts()" << std::endl;
    
    std::string sysList = m_opt -> sysNameStr();
    if(sysList==""){
        std::cout << "You should give a list of weight systematics to run over them ..." << std::endl;
        return false;
    }
    sysList += ",";
    
    size_t pos = 0;
    const std::string delimiter=",";
    while ((pos = sysList.find(delimiter)) != std::string::npos) {
        std::string current;
        current = sysList.substr(0, pos);
        std::cout << current << std::endl;
        
        if(current=="BTAGBREAK1UP")         AddToSystVector(WeightSys::BtagBreak1Up,current);
        else if(current=="BTAGBREAK2UP")    AddToSystVector(WeightSys::BtagBreak2Up,current);
        else if(current=="BTAGBREAK3UP")    AddToSystVector(WeightSys::BtagBreak3Up,current);
        else if(current=="BTAGBREAK4UP")    AddToSystVector(WeightSys::BtagBreak4Up,current);
        else if(current=="BTAGBREAK5UP")    AddToSystVector(WeightSys::BtagBreak5Up,current);
        else if(current=="BTAGBREAK6UP")    AddToSystVector(WeightSys::BtagBreak6Up,current);
        else if(current=="BTAGBREAK1DW")    AddToSystVector(WeightSys::BtagBreak1Dw,current);
        else if(current=="BTAGBREAK2DW")    AddToSystVector(WeightSys::BtagBreak2Dw,current);
        else if(current=="BTAGBREAK3DW")    AddToSystVector(WeightSys::BtagBreak3Dw,current);
        else if(current=="BTAGBREAK4DW")    AddToSystVector(WeightSys::BtagBreak4Dw,current);
        else if(current=="BTAGBREAK5DW")    AddToSystVector(WeightSys::BtagBreak5Dw,current);
        else if(current=="BTAGBREAK6DW")    AddToSystVector(WeightSys::BtagBreak6Dw,current);
        else if(current=="CTAGBREAK1UP")    AddToSystVector(WeightSys::CtagBreak1Up,current);
        else if(current=="CTAGBREAK2UP")    AddToSystVector(WeightSys::CtagBreak2Up,current);
        else if(current=="CTAGBREAK3UP")    AddToSystVector(WeightSys::CtagBreak3Up,current);
        else if(current=="CTAGBREAK4UP")    AddToSystVector(WeightSys::CtagBreak4Up,current);
        else if(current=="CTAGBREAK5UP")    AddToSystVector(WeightSys::CtagBreak5Up,current);
        else if(current=="CTAGBREAK6UP")    AddToSystVector(WeightSys::CtagBreak6Up,current);
        else if(current=="CTAGBREAK1DW")    AddToSystVector(WeightSys::CtagBreak1Dw,current);
        else if(current=="CTAGBREAK2DW")    AddToSystVector(WeightSys::CtagBreak2Dw,current);
        else if(current=="CTAGBREAK3DW")    AddToSystVector(WeightSys::CtagBreak3Dw,current);
        else if(current=="CTAGBREAK4DW")    AddToSystVector(WeightSys::CtagBreak4Dw,current);
        else if(current=="CTAGBREAK5DW")    AddToSystVector(WeightSys::CtagBreak5Dw,current);
        else if(current=="CTAGBREAK6DW")    AddToSystVector(WeightSys::CtagBreak6Dw,current);
        else { std::cout << "The systematic *" << current << "* is not known ..." << std::endl; }
        sysList.erase(0, pos + delimiter.length());
    }
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving SystManager::LoadWeightSysts()" << std::endl;
    
    return true;
}

//________________________________________________________________________
//
bool SystManager::ComputeWeights( const NtupleData *data ) const
{
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in SystManager::ComputeWeights()" << std::endl;
    
    for(unsigned int iSys = 0; iSys < m_systVector->size();++iSys){
        if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak1Up){
            if(data->finalEvent_BTag_breakdown_up && data->finalEvent_BTag_breakdown_up->size()>0)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_up->at(0);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak2Up){
            if(data->finalEvent_BTag_breakdown_up &&data->finalEvent_BTag_breakdown_up->size()>1)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_up->at(1);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak3Up){
            if(data->finalEvent_BTag_breakdown_up &&data->finalEvent_BTag_breakdown_up->size()>2)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_up->at(2);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak4Up){
            if(data->finalEvent_BTag_breakdown_up &&data->finalEvent_BTag_breakdown_up->size()>3)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_up->at(3);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak5Up){
            if(data->finalEvent_BTag_breakdown_up &&data->finalEvent_BTag_breakdown_up->size()>4)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_up->at(4);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak6Up){
            if(data->finalEvent_BTag_breakdown_up && data->finalEvent_BTag_breakdown_up->size()>5)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_up->at(5);
        } else {
            std::cout << "The systematic *"<<m_systVector->at(iSys)->name<<"* is unknown ..." << std::endl;
        }
    }
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving SystManager::ComputeWeights()" << std::endl;
    return true;
}

//________________________________________________________________________
//
bool SystManager::ClearWeights() const {
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in SystManager::ClearWeights()" << std::endl;
    
    if(m_systVector){
        for(unsigned int iEntry = 0; iEntry < m_systVector-> size(); ++iEntry){
            m_systVector -> at(iEntry) -> value = 0;
        }
    }
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving SystManager::ClearWeights()" << std::endl;
    return true;
}

//________________________________________________________________________
//
bool SystManager::Clear() const {
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Entering in SystManager::Clear()" << std::endl;
    
    if(m_systVector){
        for(unsigned int iEntry = 0; iEntry < m_systVector-> size(); ++iEntry){
            delete m_systVector -> at(iEntry);
        }
        delete m_systVector;
    }
    
    if(m_opt->msgLevel()==Debug::DEBUG) std::cout << "Leaving SystManager::Clear()" << std::endl;
    return true;
}
