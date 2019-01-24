object OptionDlg: TOptionDlg
  Left = 318
  Top = 168
  HelpContext = 5
  BorderStyle = bsDialog
  Caption = #12458#12503#12471#12519#12531#35373#23450
  ClientHeight = 331
  ClientWidth = 474
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  ShowHint = True
  OnCreate = FormCreate
  OnShow = FormShow
  DesignSize = (
    474
    331)
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 115
    Top = 296
    Width = 80
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 219
    Top = 296
    Width = 80
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 1
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 474
    Height = 281
    HelpContext = 5
    ActivePage = TabSheet1
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
    TabWidth = 90
    object TabSheet1: TTabSheet
      HelpContext = 5
      Caption = #21462#12426#36796#12415
      DesignSize = (
        466
        253)
      object Label5: TLabel
        Left = 438
        Top = 18
        Width = 13
        Height = 13
        Caption = 'ms'
      end
      object Label6: TLabel
        Left = 438
        Top = 50
        Width = 13
        Height = 13
        Caption = 'ms'
      end
      object SndGroupBox: TGroupBox
        Left = 8
        Top = 87
        Width = 450
        Height = 87
        Anchors = [akLeft, akTop, akRight]
        Caption = #12469#12454#12531#12489
        TabOrder = 3
        DesignSize = (
          450
          87)
        object Label3: TLabel
          Left = 12
          Top = 26
          Width = 71
          Height = 15
          Alignment = taRightJustify
          AutoSize = False
          Caption = #23550#35937#22793#21270#26178
        end
        object Label4: TLabel
          Left = 12
          Top = 58
          Width = 71
          Height = 15
          Alignment = taRightJustify
          AutoSize = False
          Caption = #30906#23450#26178
        end
        object ChgSndEdit: TEdit
          Left = 87
          Top = 23
          Width = 304
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
        object RefChgSndBtn: TButton
          Left = 393
          Top = 23
          Width = 30
          Height = 22
          Hint = #21442#29031
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefSndBtnClick
        end
        object CapSndEdit: TEdit
          Tag = 1
          Left = 87
          Top = 55
          Width = 304
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 3
        end
        object RefCapSndBtn: TButton
          Tag = 1
          Left = 393
          Top = 55
          Width = 30
          Height = 22
          Hint = #21442#29031
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 4
          OnClick = RefSndBtnClick
        end
        object TstChgSndBtn: TButton
          Left = 425
          Top = 23
          Width = 20
          Height = 22
          Hint = #12486#12473#12488
          Anchors = [akTop, akRight]
          Caption = '>'
          TabOrder = 2
          OnClick = TestSndBtnClick
        end
        object TstCapSndBtn: TButton
          Tag = 1
          Left = 425
          Top = 55
          Width = 20
          Height = 22
          Hint = #12486#12473#12488
          Anchors = [akTop, akRight]
          Caption = '>'
          TabOrder = 5
          OnClick = TestSndBtnClick
        end
      end
      object AllIconCheckBox: TCheckBox
        Left = 24
        Top = 198
        Width = 329
        Height = 17
        Caption = #12489#12525#12483#12503#26178#12395#20840#12390#12398#12450#12452#12467#12531#12434#21462#12426#36796#12416
        TabOrder = 4
      end
      object CapWaitEdit: TLabeledEdit
        Left = 373
        Top = 47
        Width = 60
        Height = 21
        Alignment = taRightJustify
        EditLabel.Width = 173
        EditLabel.Height = 13
        EditLabel.Caption = 'Ctrl'#25276#12375#19979#12370#26178#12398#21462#12426#36796#12415#36933#24310#26178#38291
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 2
      end
      object HideWaitEdit: TLabeledEdit
        Left = 373
        Top = 15
        Width = 60
        Height = 21
        Alignment = taRightJustify
        EditLabel.Width = 154
        EditLabel.Height = 13
        EditLabel.Caption = #21462#12426#36796#12415#26178#12395#30011#38754#12364#38560#12428#12427#26178#38291
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 1
      end
      object CurGroupBox: TGroupBox
        Left = 8
        Top = 8
        Width = 150
        Height = 62
        Caption = #26454
        TabOrder = 0
        object Image1: TImage
          Left = 34
          Top = 20
          Width = 32
          Height = 32
          Picture.Data = {
            0954506E67496D61676589504E470D0A1A0A0000000D49484452000000200000
            00200806000000737A7AF4000000086163544C0000000100000000B42DE9A000
            00001A6663544C000000000000002000000020000000000000000000A703E801
            00C488D3BD000000A24944415478DAEDD74B0E80200C04507BFF432321310A51
            E967A66C6047D0E14511AA1C8B9B0CFDF233460794DAEE019114C43541377926
            6203666BE0ED3A1AA021BA4E053D2014C42CB4B0119A402A421B464358822808
            6B081CE1098022BC37C310113D04117D87610462258710A8EFD98D40EE6A2E04
            7A6F372318279C09C13AE7D50866B5A342B0ABDE2922A3F6FF45A4FC7C0C8825
            808658F9043EDB062C079C115C662123AE57CC0000000049454E44AE426082}
        end
        object Image2: TImage
          Left = 100
          Top = 20
          Width = 32
          Height = 32
          Picture.Data = {
            0954506E67496D61676589504E470D0A1A0A0000000D49484452000000200000
            00200804000000D973B27F000000097048597300000F6100000F6101A83FA769
            0000008F4944415478DADDD4490E85400804D0E2FE87467FBE26B68C45EF6465
            62F182F62050AC25A04AA0BAF6CE81F3F1D73D05AEF629F06F1F4FF0689F00C0
            0E21661949E21D55760A1B24092F46117E8820A2489B88034D227BDD2272BF41
            54DF5812F57F2E88CE5AA7446FBF254477CF8744FFDC050473F65D82BB7F1C82
            BCC02CC10286E0819BD80000DD9B60A90F00073A303D21F360BAF00000000049
            454E44AE426082}
        end
        object CurRadioBtn1: TRadioButton
          Left = 12
          Top = 28
          Width = 17
          Height = 17
          TabOrder = 0
        end
        object CurRadioBtn2: TRadioButton
          Left = 81
          Top = 28
          Width = 17
          Height = 17
          TabOrder = 1
        end
      end
    end
    object TabSheet2: TTabSheet
      HelpContext = 5
      Caption = #12467#12500#12540#21205#20316
      ImageIndex = 1
      DesignSize = (
        466
        253)
      object GroupBox2: TGroupBox
        Left = 8
        Top = 8
        Width = 450
        Height = 113
        Anchors = [akLeft, akTop, akRight]
        Caption = #12450#12463#12486#12451#12502#12395#12377#12427#12454#12451#12531#12489#12454
        TabOrder = 0
        object ActClassEdit: TLabeledEdit
          Left = 136
          Top = 23
          Width = 305
          Height = 21
          EditLabel.Width = 90
          EditLabel.Height = 13
          EditLabel.Caption = #12454#12451#12531#12489#12454#12398#12463#12521#12473#21517
          LabelPosition = lpLeft
          TabOrder = 0
        end
        object ActTitleEdit: TLabeledEdit
          Left = 136
          Top = 52
          Width = 305
          Height = 21
          EditLabel.Width = 91
          EditLabel.Height = 13
          EditLabel.Caption = #12479#12452#12488#12523'('#21069#26041#19968#33268')'
          LabelPosition = lpLeft
          TabOrder = 1
        end
        object ActTitle2Edit: TLabeledEdit
          Left = 136
          Top = 81
          Width = 305
          Height = 21
          EditLabel.Width = 91
          EditLabel.Height = 13
          EditLabel.Caption = #12479#12452#12488#12523'('#24460#26041#19968#33268')'
          LabelPosition = lpLeft
          TabOrder = 2
        end
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 136
        Width = 450
        Height = 54
        Anchors = [akLeft, akTop, akRight]
        Caption = #12454#12451#12531#12489#12454#12364#28961#12356#22580#21512#12395#36215#21205#12377#12427#12503#12525#12464#12521#12512
        TabOrder = 1
        DesignSize = (
          450
          54)
        object RefCpyCmdBtn: TButton
          Left = 411
          Top = 22
          Width = 30
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefCmdBtnClick
        end
        object CpyCmdEdit: TEdit
          Left = 10
          Top = 22
          Width = 399
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
      end
      object SendKeyEdit: TLabeledEdit
        Left = 144
        Top = 206
        Width = 273
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 13
        EditLabel.Caption = #36865#20986#12461#12540
        LabelPosition = lpLeft
        TabOrder = 2
      end
    end
    object TabSheet3: TTabSheet
      HelpContext = 5
      Caption = #20445#23384#21205#20316
      ImageIndex = 2
      DesignSize = (
        466
        253)
      object Label10: TLabel
        Left = 16
        Top = 20
        Width = 74
        Height = 13
        Caption = 'Jpeg'#20445#23384#21697#36074' '
      end
      object JpgLabel: TLabel
        Left = 112
        Top = 20
        Width = 28
        Height = 15
        Alignment = taRightJustify
        AutoSize = False
        Caption = '100'
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 73
        Width = 450
        Height = 54
        Anchors = [akLeft, akTop, akRight]
        Caption = #20445#23384#24460#12395#36215#21205#12377#12427#12503#12525#12464#12521#12512
        TabOrder = 1
        DesignSize = (
          450
          54)
        object RefSavCmdBtn: TButton
          Tag = 1
          Left = 412
          Top = 22
          Width = 30
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefCmdBtnClick
        end
        object SavCmdEdit: TEdit
          Left = 8
          Top = 22
          Width = 402
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
      end
      object JpgTrackBar: TTrackBar
        Left = 142
        Top = 16
        Width = 200
        Height = 33
        Max = 100
        Min = 1
        Frequency = 10
        Position = 1
        TabOrder = 0
        OnChange = JpgTrackBarChange
      end
    end
    object TabSheet4: TTabSheet
      Caption = #12381#12398#20182
      ImageIndex = 3
      object GroupBox5: TGroupBox
        Left = 8
        Top = 8
        Width = 260
        Height = 54
        Caption = #12469#12452#12474#22793#26356#12398#12450#12523#12468#12522#12474#12512
        TabOrder = 0
        DesignSize = (
          260
          54)
        object ScaleOptComboBox: TComboBox
          Left = 8
          Top = 22
          Width = 243
          Height = 21
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 344
    Top = 282
  end
end
