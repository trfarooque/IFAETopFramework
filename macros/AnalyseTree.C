/*
 
 ANALYSETREE.C
 ==============
 Dumps information about the trees contained in a ROOT file.
 
 To run it:
      root -l -b -q 'AnalyseTree.C+("myRootFile.root")'
 
 It creates categories based on the presence of an "_" sign in the name of the branches.
 
 The outputs are .eps file, as well as printed values of the results on the screen.
 
 Contact: Loic Valery <lvalery@cern.ch>
 
 */

#include <iostream>
#include <map>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TIterator.h"
#include "TList.h"
#include "TCanvas.h"
#include "TPie.h"
#include "TKey.h"
#include "TH1F.h"
#include "TROOT.h"

//Functions
long GetTreeSizeStudy( TTree* );


//__________________________________________________________________________
//
int AnalyseTree( const std::string &fileName ){
    
    std::cout << "==> Analysing trees located in the file: " << fileName << std::endl;
    
    TFile *f = new TFile( fileName.c_str(), "read");
    if(!f){
        std::cerr << "<!> ERROR: the file cannot be opened. Please check." << std::endl;
        return -1;
    }
    
    //
    // Loop over the content of the file and studies all trees
    //
    TList *list = f -> GetListOfKeys();
    TIterator *it = list -> MakeIterator();
    
    std::map < std::string, long > map_sizes;
    while ( TKey *key = (TKey*)it -> Next() ){
        TObject *obj = (TObject*)key -> ReadObj();
        
        if( obj -> IsA() == TTree::Class() ){
            TTree *tree = (TTree*)obj -> Clone();
            long size = GetTreeSizeStudy( tree );
            map_sizes.insert( std::pair< std::string, long>(obj->GetName(),size) );
        }
    }
    
    return 1;
    
}

//__________________________________________________________________________
//
long GetTreeSizeStudy( TTree* tree ){
    
    const double threshold = 0.01;
    
    TObjArray* list = tree -> GetListOfBranches();
    TIterator *it = list -> MakeIterator();
    std::map < std::string, long > map_categories;
    long totalSize = 0;
    
    //Creating the pdf
    TCanvas c;
    TH1F* h = new TH1F();
    h->SetName("h_temp");
    h->SetTitle("checks_for_trees");
    c.cd();
    h->Draw();
    c.SaveAs(Form("%s.pdf[",(tree->GetName())));
    
    TCanvas c1;
    while ( TBranch *branch = (TBranch*)it -> Next() ){
        //Categories
        std::string name = branch -> GetName();
        size_t pos = name.find("_");
        std::string category = name.substr(0, pos);
        if(pos==std::string::npos) category = "Single variables";
        tree->Draw( Form( "%s >> h_temp_%s",branch->GetName(), branch->GetName() ),"","hist");
        c1.SaveAs(Form("%s.pdf",(tree->GetName())));
        
        //Size
        long size = branch -> GetZipBytes();
        totalSize += size;
        
        //Pushes in the map if necessary
        if(map_categories.find(category) != map_categories.end()){
            map_categories[category] += size;
        } else {
            map_categories.insert( std::pair < std::string, long >( category, size) );
        }
    }
    
    c.SaveAs(Form("%s.pdf]",(tree->GetName())));
    
    //
    // Now creates the pie chart
    //
    double vals[map_categories.size()];
    int colors[map_categories.size()];
    int counter = 0;
    
    std::cout << "===============================================================" << std::endl;
    std::cout << "Information about tree: " << tree -> GetName() << std::endl;
    std::cout << "    Size: " << std::setw(5) << totalSize / (1024.*1024.) << " Mo" << std::endl;
    std::cout << "===============================================================" << std::endl;
    
    for ( std::pair < std::string, long > Pair : map_categories ) {
        if( (double)Pair.second/(totalSize) < threshold ) continue;
        vals[counter] = (double)(Pair.second);
        colors[counter] = counter;
        std::cout << std::setw(4) << counter << "     " << std::setw(20) << Pair.first << " ---> " << std::setw(3) << Pair.second/(1024.*1024.) << " Mo   (" << std::setw(4) << (double)Pair.second/(totalSize)*100. << " %)" << std::endl;
        counter++;
    }
    std::cout << "===============================================================" << std::endl;
    const int nvals = counter;
    
    //
    // Drawing the piechart
    //
    TString c_name = "c_";
    c_name += tree -> GetName();
    TCanvas *cpie = new TCanvas(c_name,"TPie test",700,700);
    
    TString title = "#splitline{Tree: ";
    title += tree -> GetName();
    title += "}{";
    title += "Size: ";
    title += Form("%.1f",totalSize/(1024.*1024.));
    title += " Mo}";

    TPie *pie = new TPie(tree->GetName(),title,nvals,vals,colors);
    pie -> SetRadius(.3);
    pie -> SetLabelsOffset(.01);
    pie -> SetTextSize(0.03);
    pie -> SetLabelFormat("#splitline{%perc}{%txt}");
    
    //
    // Adding the legends
    //
    counter = 0;
    for ( std::pair < std::string, long > Pair : map_categories ) {
        if( (double)Pair.second/(totalSize) < threshold ) continue;
        pie -> SetEntryLabel(counter,Pair.first.c_str());
        counter++;
    }
    
    pie -> Draw();
    
    //
    // Title of the output
    //
    TString name = tree->GetName();
    name += ".eps";
    name.ReplaceAll(" ","_");
    cpie->Print(name);
    
    return totalSize;
}