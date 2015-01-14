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
        
        else if(current=="LTAGBREAK1UP")    AddToSystVector(WeightSys::LtagBreak1Up,current);
        else if(current=="LTAGBREAK2UP")    AddToSystVector(WeightSys::LtagBreak2Up,current);
        else if(current=="LTAGBREAK3UP")    AddToSystVector(WeightSys::LtagBreak3Up,current);
        else if(current=="LTAGBREAK4UP")    AddToSystVector(WeightSys::LtagBreak4Up,current);
        else if(current=="LTAGBREAK5UP")    AddToSystVector(WeightSys::LtagBreak5Up,current);
        else if(current=="LTAGBREAK6UP")    AddToSystVector(WeightSys::LtagBreak6Up,current);
        else if(current=="LTAGBREAK7UP")    AddToSystVector(WeightSys::LtagBreak7Up,current);
        else if(current=="LTAGBREAK8UP")    AddToSystVector(WeightSys::LtagBreak8Up,current);
        else if(current=="LTAGBREAK9UP")    AddToSystVector(WeightSys::LtagBreak9Up,current);
        else if(current=="LTAGBREAK10UP")    AddToSystVector(WeightSys::LtagBreak10Up,current);
        else if(current=="LTAGBREAK11UP")    AddToSystVector(WeightSys::LtagBreak11Up,current);
        else if(current=="LTAGBREAK12UP")    AddToSystVector(WeightSys::LtagBreak12Up,current);
        else if(current=="LTAGBREAK1DW")    AddToSystVector(WeightSys::LtagBreak1Dw,current);
        else if(current=="LTAGBREAK2DW")    AddToSystVector(WeightSys::LtagBreak2Dw,current);
        else if(current=="LTAGBREAK3DW")    AddToSystVector(WeightSys::LtagBreak3Dw,current);
        else if(current=="LTAGBREAK4DW")    AddToSystVector(WeightSys::LtagBreak4Dw,current);
        else if(current=="LTAGBREAK5DW")    AddToSystVector(WeightSys::LtagBreak5Dw,current);
        else if(current=="LTAGBREAK6DW")    AddToSystVector(WeightSys::LtagBreak6Dw,current);
        else if(current=="LTAGBREAK7DW")    AddToSystVector(WeightSys::LtagBreak7Dw,current);
        else if(current=="LTAGBREAK8DW")    AddToSystVector(WeightSys::LtagBreak8Dw,current);
        else if(current=="LTAGBREAK9DW")    AddToSystVector(WeightSys::LtagBreak9Dw,current);
        else if(current=="LTAGBREAK10DW")   AddToSystVector(WeightSys::LtagBreak10Dw,current);
        else if(current=="LTAGBREAK11DW")   AddToSystVector(WeightSys::LtagBreak11Dw,current);
        else if(current=="LTAGBREAK12DW")   AddToSystVector(WeightSys::LtagBreak12Dw,current);
        else if(current=="BTAGEXTRAPUP")    AddToSystVector(WeightSys::BtagExtrapUp,current);
        else if(current=="BTAGEXTRAPDW")    AddToSystVector(WeightSys::BtagExtrapDw,current);
        
        else if(current=="MUONTRIGSFUP")    AddToSystVector(WeightSys::MuonTrigSFUp,current);
        else if(current=="MUONTRIGSFDW")    AddToSystVector(WeightSys::MuonTrigSFDw,current);
        else if(current=="MUONRECOSFUP")    AddToSystVector(WeightSys::MuonRecoSFUp,current);
        else if(current=="MUONRECOSFDW")    AddToSystVector(WeightSys::MuonRecoSFDw,current);
        else if(current=="MUONIDSFUP")      AddToSystVector(WeightSys::MuonIdSFUp,current);
        else if(current=="MUONIDSFDW")      AddToSystVector(WeightSys::MuonIdSFDw,current);
        else if(current=="ELECTRONTRIGSFUP")AddToSystVector(WeightSys::ElectronTrigSFUp,current);
        else if(current=="ELECTRONTRIGSFDW")AddToSystVector(WeightSys::ElectronTrigSFDw,current);
        else if(current=="ELECTRONRECOSFUP")AddToSystVector(WeightSys::ElectronRecoSFUp,current);
        else if(current=="ELECTRONRECOSFDW")AddToSystVector(WeightSys::ElectronRecoSFDw,current);
        else if(current=="ELECTRONIDSFUP")  AddToSystVector(WeightSys::ElectronIdSFUp,current);
        else if(current=="ELECTRONIDSFDW")  AddToSystVector(WeightSys::ElectronIdSFDw,current);
        
        else if(current=="TTHSCALESTATIC")  AddToSystVector(WeightSys::ttH_scaleStatic,current);
        else if(current=="TTHSCALEUP")      AddToSystVector(WeightSys::ttH_scaleUp,current);
        else if(current=="TTHSCALEDW")    AddToSystVector(WeightSys::ttH_scaleDw,current);
        
        else if(current=="TTBARRWISRUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_ISRUp,current);
        else if(current=="TTBARRWISRDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_ISRDw,current);
        else if(current=="TTBARRWFRAGMENTATIONUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_FragmentationUp,current);
        else if(current=="TTBARRWFRAGMENTATIONDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_FragmentationDw,current);
        else if(current=="TTBARRWMCGENUP")   AddToSystVector(WeightSys::ttbarTopPtDataWeight_MCgenUp,current);
        else if(current=="TTBARRWMCGENDW")   AddToSystVector(WeightSys::ttbarTopPtDataWeight_MCgenDw,current);
        else if(current=="TTBARRWJERUP")     AddToSystVector(WeightSys::ttbarTopPtDataWeight_JERUp,current);
        else if(current=="TTBARRWJERDW")     AddToSystVector(WeightSys::ttbarTopPtDataWeight_JERDw,current);
        else if(current=="TTBARRWBJESUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_bJESUp,current);
        else if(current=="TTBARRWBJESDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_bJESDw,current);
        else if(current=="TTBARRWCLOSEBYJESUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_CloseByJESUp,current);
        else if(current=="TTBARRWCLOSEBYJESDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_CloseByJESDw,current);
        else if(current=="TTBARRWETACALIBJESUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_EtaCalibJESUp,current);
        else if(current=="TTBARRWETACALIBJESDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_EtaCalibJESDw,current);
        else if(current=="TTBARRWEFFDETJESUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_effdetset1JESUp,current);
        else if(current=="TTBARRWEFFDETJESDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_effdetset1JESDw,current);
        else if(current=="TTBARRWBTAGEFFUP")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_BtagEffUp,current);
        else if(current=="TTBARRWBTAGEFFDW")    AddToSystVector(WeightSys::ttbarTopPtDataWeight_BtagEffDw,current);
        else if(current=="TTBARTOPPTONLY")    AddToSystVector(WeightSys::ttbar_toppt_only,current);
        else if(current=="TTBARTTBARPTONLY")    AddToSystVector(WeightSys::ttbar_ttbarpt_only,current);
        
        else if(current=="TTBARMGMCUP")    AddToSystVector(WeightSys::ttbar_MadgraphMcUp,current);
        else if(current=="TTBARMGMCDW")    AddToSystVector(WeightSys::ttbar_MadgraphMcDw,current);
        else if(current=="TTBARMGMBUP")    AddToSystVector(WeightSys::ttbar_MadgraphMbUp,current);
        else if(current=="TTBARMGMBDW")    AddToSystVector(WeightSys::ttbar_MadgraphMbDw,current);
        else if(current=="TTBARMGQ2UP")    AddToSystVector(WeightSys::ttbar_MadgraphQ2Up,current);
        else if(current=="TTBARMGQ2DW")    AddToSystVector(WeightSys::ttbar_MadgraphQ2Dw,current);
        else if(current=="TTBARMGMATCH")    AddToSystVector(WeightSys::ttbar_MadgraphMatch,current);
        else if(current=="TTBARMGPP")    AddToSystVector(WeightSys::ttbar_MadgraphPP,current);
        
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
        // B UP
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
        }
        // B DOWN
        else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak1Dw){
            if(data->finalEvent_BTag_breakdown_down && data->finalEvent_BTag_breakdown_down->size()>0)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_down->at(0);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak2Dw){
            if(data->finalEvent_BTag_breakdown_down &&data->finalEvent_BTag_breakdown_down->size()>1)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_down->at(1);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak3Dw){
            if(data->finalEvent_BTag_breakdown_down &&data->finalEvent_BTag_breakdown_down->size()>2)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_down->at(2);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak4Dw){
            if(data->finalEvent_BTag_breakdown_down &&data->finalEvent_BTag_breakdown_down->size()>3)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_down->at(3);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak5Dw){
            if(data->finalEvent_BTag_breakdown_down &&data->finalEvent_BTag_breakdown_down->size()>4)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_down->at(4);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagBreak6Dw){
            if(data->finalEvent_BTag_breakdown_down && data->finalEvent_BTag_breakdown_down->size()>5)
                m_systVector->at(iSys)->value = data->finalEvent_BTag_breakdown_down->at(5);
        }
        // C UP
        else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak1Up){
            if(data->finalEvent_CTag_breakdown_up && data->finalEvent_CTag_breakdown_up->size()>0)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_up->at(0);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak2Up){
            if(data->finalEvent_CTag_breakdown_up &&data->finalEvent_CTag_breakdown_up->size()>1)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_up->at(1);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak3Up){
            if(data->finalEvent_CTag_breakdown_up &&data->finalEvent_CTag_breakdown_up->size()>2)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_up->at(2);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak4Up){
            if(data->finalEvent_CTag_breakdown_up &&data->finalEvent_CTag_breakdown_up->size()>3)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_up->at(3);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak5Up){
            if(data->finalEvent_CTag_breakdown_up &&data->finalEvent_CTag_breakdown_up->size()>4)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_up->at(4);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak6Up){
            if(data->finalEvent_CTag_breakdown_up && data->finalEvent_CTag_breakdown_up->size()>5)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_up->at(5);
        }
        // C DOWN
        else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak1Dw){
            if(data->finalEvent_CTag_breakdown_down && data->finalEvent_CTag_breakdown_down->size()>0)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_down->at(0);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak2Dw){
            if(data->finalEvent_CTag_breakdown_down &&data->finalEvent_CTag_breakdown_down->size()>1)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_down->at(1);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak3Dw){
            if(data->finalEvent_CTag_breakdown_down &&data->finalEvent_CTag_breakdown_down->size()>2)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_down->at(2);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak4Dw){
            if(data->finalEvent_CTag_breakdown_down &&data->finalEvent_CTag_breakdown_down->size()>3)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_down->at(3);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak5Dw){
            if(data->finalEvent_CTag_breakdown_down &&data->finalEvent_CTag_breakdown_down->size()>4)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_down->at(4);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::CtagBreak6Dw){
            if(data->finalEvent_CTag_breakdown_down && data->finalEvent_CTag_breakdown_down->size()>5)
                m_systVector->at(iSys)->value = data->finalEvent_CTag_breakdown_down->at(5);
        }
        // L UP
        else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak1Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>0)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(0);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak2Up){
            if(data->finalEvent_LTag_breakdown_up &&data->finalEvent_LTag_breakdown_up->size()>1)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(1);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak3Up){
            if(data->finalEvent_LTag_breakdown_up &&data->finalEvent_LTag_breakdown_up->size()>2)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(2);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak4Up){
            if(data->finalEvent_LTag_breakdown_up &&data->finalEvent_LTag_breakdown_up->size()>3)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(3);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak5Up){
            if(data->finalEvent_LTag_breakdown_up &&data->finalEvent_LTag_breakdown_up->size()>4)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(4);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak6Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>5)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(5);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak7Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>6)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(6);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak8Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>7)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(7);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak9Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>8)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(8);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak10Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>9)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(9);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak11Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>10)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(10);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak12Up){
            if(data->finalEvent_LTag_breakdown_up && data->finalEvent_LTag_breakdown_up->size()>11)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_up->at(11);
        }
        // L DOWN
        else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak1Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>0)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(0);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak2Dw){
            if(data->finalEvent_LTag_breakdown_down &&data->finalEvent_LTag_breakdown_down->size()>1)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(1);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak3Dw){
            if(data->finalEvent_LTag_breakdown_down &&data->finalEvent_LTag_breakdown_down->size()>2)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(2);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak4Dw){
            if(data->finalEvent_LTag_breakdown_down &&data->finalEvent_LTag_breakdown_down->size()>3)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(3);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak5Dw){
            if(data->finalEvent_LTag_breakdown_down &&data->finalEvent_LTag_breakdown_down->size()>4)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(4);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak6Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>5)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(5);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak7Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>6)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(6);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak8Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>7)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(7);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak9Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>8)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(8);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak10Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>9)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(9);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak11Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>10)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(10);
        } else if(m_systVector->at(iSys)->sysType==WeightSys::LtagBreak12Dw){
            if(data->finalEvent_LTag_breakdown_down && data->finalEvent_LTag_breakdown_down->size()>11)
                m_systVector->at(iSys)->value = data->finalEvent_LTag_breakdown_down->at(11);
        }
        //BTAG EXTRAP UP
        else if(m_systVector->at(iSys)->sysType==WeightSys::BtagExtrapUp){
            m_systVector->at(iSys)->value = data -> finalEvent_BTagExtrap_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::BtagExtrapDw){
            m_systVector->at(iSys)->value = data -> finalEvent_BTagExtrap_down;
        }
        //MUON SCALE FACTORS
        else if(m_systVector->at(iSys)->sysType==WeightSys::MuonTrigSFUp){
            m_systVector->at(iSys)->value = data -> finalEvent_MuonTrig_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::MuonTrigSFDw){
            m_systVector->at(iSys)->value = data -> finalEvent_MuonTrig_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::MuonRecoSFUp){
            m_systVector->at(iSys)->value = data -> finalEvent_MuonReco_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::MuonRecoSFDw){
            m_systVector->at(iSys)->value = data -> finalEvent_MuonReco_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::MuonIdSFUp){
            m_systVector->at(iSys)->value = data -> finalEvent_MuonID_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::MuonIdSFDw){
            m_systVector->at(iSys)->value = data -> finalEvent_MuonID_down;
        }
        //ELECTRON SCALE FACTORS
        else if(m_systVector->at(iSys)->sysType==WeightSys::ElectronTrigSFUp){
            m_systVector->at(iSys)->value = data -> finalEvent_EleTrig_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ElectronTrigSFDw){
            m_systVector->at(iSys)->value = data -> finalEvent_EleTrig_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ElectronRecoSFUp){
            m_systVector->at(iSys)->value = data -> finalEvent_EleReco_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ElectronRecoSFDw){
            m_systVector->at(iSys)->value = data -> finalEvent_EleReco_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ElectronIdSFUp){
            m_systVector->at(iSys)->value = data -> finalEvent_EleID_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ElectronIdSFDw){
            m_systVector->at(iSys)->value = data -> finalEvent_EleID_down;
        }
        //TTH SCALE UNCERTAINTIES
        else if(m_systVector->at(iSys)->sysType==WeightSys::ttH_scaleStatic){
            m_systVector->at(iSys)->value = data -> finalEvent_ttH_scale_static;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttH_scaleUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttH_scale_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttH_scaleDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttH_scale_down;
        }
        //TTBAR TOP/TTBAR PT REWEIGHTING UNCERTAINTIES
        else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_ISRUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_IFSR_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_ISRDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_IFSR_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_FragmentationUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_Fragmentation_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_FragmentationDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_Fragmentation_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_MCgenUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_MCgen_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_MCgenDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_MCgen_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_JERUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_JER_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_JERDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_JER_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_bJESUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_bJES_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_bJESDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_bJES_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_CloseByJESUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_closebyJES_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_CloseByJESDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_closebyJES_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_EtaCalibJESUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_etacalibJES_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_EtaCalibJESDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_etacalibJES_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_effdetset1JESUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_effdetset1JES_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_effdetset1JESDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_effdetset1JES_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_BtagEffUp){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_btageff_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbarTopPtDataWeight_BtagEffDw){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbartop_pt_rw_btageff_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_toppt_only){
            m_systVector->at(iSys)->value = data -> finalEvent_toppt_only;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_ttbarpt_only){
            m_systVector->at(iSys)->value = data -> finalEvent_ttbarpt_only;
        }
        // TTBAR CC REWEIGHTING UNCERTATINTIES
        else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphMcUp){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphMC_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphMcDw){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphMC_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphMbUp){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphMB_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphMbDw){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphMB_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphQ2Up){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphQ2_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphQ2Dw){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphQ2_down;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphMatch){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphMatch_up;
        } else if(m_systVector->at(iSys)->sysType==WeightSys::ttbar_MadgraphPP){
            m_systVector->at(iSys)->value = data -> finalEvent_MadgraphPP;
        }
        else {
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
