{
    gSystem->Load("libPWG2LRC");
Int_t errybins = 20000;
	Double_t errymin = 0.000025,
		  errymax = 1.000025;

 	TH1D *gaus = new TH1D("source","source hist",1000,-5,5);
    gaus->FillRandom("gaus",10000);
    TH2D *source = new TH2D("PtN","PtN Test",100,0,5,100,0,5);
    TH2D *err = new TH2D("err","errl hist",100,0,5,errybins,errymin,errymax);
	Double_t x, y;
    for (Int_t i=0;i<10000;i++) 
	{   
	    x =(gaus->GetRandom()+5)/2.0;
	    y = 0.678 * (x + gaus->GetRandom());
	    //Create source 2D histogram with correlation coefficient 0.678
		source->Fill(x,y);
		err->Fill(x,1/((gaus->GetRandom()+5)));
    }
    //Create TNN class encapsulated NN correlation algorithms
    //2D histogram pass into TNN constructor
	AliLRCPtPt final1("name", source, 0.35, err);

   TCanvas *c1 = new TCanvas("c1","c1",800,1000);
	c1->Divide(3);
	c1->cd(1);
	source->Draw();
	c1->cd(2);
	final1.Draw_abs();
	c1->cd(3);
	final1.Draw_rel();
	c1->cd();

	AliLRCPtPt final2;
        final2.MakeHistogramm("name", source, 0.35, err);

   TCanvas *c2 = new TCanvas("c2","c2",800,1000);
	c2->Divide(3);
	c2->cd(1);
	source->Draw();
	c2->cd(2);
	final2.Draw_abs();
	c2->cd(3);
	final2.Draw_rel();
	c2->cd();

}