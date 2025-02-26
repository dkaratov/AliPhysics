void AddTaskGammaV2(
    TString period = "",
    TString trainConfig = "0" // additional counter for trainconfig
)
{

    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr)
    {
        return;
    }
    AliVEventHandler *inputHandler = mgr->GetInputEventHandler();
    AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
    AliAnalysisTaskGammaCaloV2 *fGammaCalo = nullptr;
    fGammaCalo = (AliAnalysisTaskGammaCaloV2 *)mgr->GetTask(Form("GammaCalo_%i", trainConfig.Atoi()));
    if (!fGammaCalo)
    {
        cout << "===============GammaCalo not found!!!!===================" << endl;
        return;
    }
    TList *EventCutList = new TList();
    EventCutList->SetOwner(kTRUE);
    Int_t nCuts = fGammaCalo->GetCutsNumber();
    EventCutList = fGammaCalo->GetEventCutList();
    //  Double_t cent = fGammaCalo->GetCent();//
    TString fileName = AliAnalysisManager::GetCommonFileName();
    fileName += ":MyTask"; // create a subfolder in the file
    // now we create an instance of your task
    AliAnalysisTaskPi0EtaV2 *task = new AliAnalysisTaskPi0EtaV2("MyTask"); //
    if (!task)
    {
        return;
    }
    TFile *fVZEROCalibFile = nullptr;
    TList *fVZEROCalibList = nullptr;

    if (!gGrid)
        TGrid::Connect("alien://");

    if (period.EqualTo("LHC15o"))
    {
        //    fVZEROCalibFile = TFile::Open("alien:///alice/cern.ch/user/c/chunzhen/CalibFiles/LHC15o/VZEROCalibFile15o.root", "READ");
        fVZEROCalibFile = TFile::Open("/Users/mojie/Works/legotrain_helpers-master/VZEROCalibFile15o.root", "READ");
        fVZEROCalibList = dynamic_cast<TList *>(fVZEROCalibFile->Get("VZEROCalibList"));
    }
    if (period.EqualTo("LHC18q"))
    {
        fVZEROCalibFile = TFile::Open("alien:///alice/cern.ch/user/c/chunzhen/CalibFiles/LHC18q/calibq2V0C18qP3.root", "READ");
        fVZEROCalibList = dynamic_cast<TList *>(fVZEROCalibFile->Get("18qlistspPerc"));
    }
    if (period.EqualTo("LHC18r"))
    {
        fVZEROCalibFile = TFile::Open("alien:///alice/cern.ch/user/c/chunzhen/CalibFiles/LHC18r/calibq2V0C18rP3.root", "READ");
        fVZEROCalibList = dynamic_cast<TList *>(fVZEROCalibFile->Get("18rlistspPerc"));
    }
    if (fVZEROCalibList)
    {
        task->SetListForVZEROCalib(fVZEROCalibList);
        std::cout << "================  VZERO List Set in GammaTest =================" << std::endl;
    }
    else
        std::cout << "!!!!!!!!!!!!!!!VZERO List not Found!!!!!!!!!!!!!!!" << std::endl;

    task->SetnCuts(nCuts);
    //  task->SetCent(cent);
    task->SetPeriod(period);
    task->SetCutList(EventCutList);
    mgr->AddTask(task);

    //  mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());
    // same for the output

    AliAnalysisDataContainer *coutput_result = mgr->CreateContainer("ListResults", TList::Class(),
                                                                    AliAnalysisManager::kOutputContainer,
                                                                    fileName.Data());
    mgr->ConnectInput(task, 0, cinput);
    mgr->ConnectOutput(task, 1, coutput_result);

    //  mgr->ConnectOutput(task,1,mgr->CreateContainer("MyOutputContainer", TList::Class(), AliAnalysisManager::kOutputContainer, fileName.Data()));
    // in the end, this macro returns a pointer to your task. this will be convenient later on
    // when you will run your analysis in an analysis train on grid
    return;
} /// 111
