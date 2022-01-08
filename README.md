# leagueofquartet_encountradar
## 旧Siv3D(Siv3D(August2016v2))のバージョンで遊べる2DのRPGです。以下のURLに記載されている方法で旧Siv3Dをダウンロードしてください。<br>
https://github.com/Siv3D/Reference-JP/wiki/%E3%83%80%E3%82%A6%E3%83%B3%E3%83%AD%E3%83%BC%E3%83%89%E3%81%A8%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB<br>
Visual Studio を起動し、Siv3D(August2016v2) のプロジェクトを新規作成します。そしてプロジェクト名のフォルダにアップロードされているcpp、hppファイル、Imageフォルダ、Soundフォルダ、ASCフォルダを入れて下さい。但し、デフォルトのSiv3Dで実装した場合、「変数m_data、関数changeScene()がアクセスできません」というエラーが出ると思います。そのためSiv3Dの既存ライブラリであるSceneManager.hppを開いてください。そしてそのファイルのSceneBaseクラスのprotectedに宣言されているスマートポインタ変数のm_dataとbool型の関数changeScene()をpublicに移行させて下さい。そうすれば、エラーが解決され遊べると思います。

