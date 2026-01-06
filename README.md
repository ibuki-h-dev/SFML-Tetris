1, プロジェクト名 : SFML-Tetris<br>
2, 開発動機 : <br>
      *これまでatcoderでのアルゴリズムの勉強に力を入れてきたが、<br>
      これからは開発もしようと思い立ったため、<br>
      本やAIを用いてゲーム作成の基礎を学ぶ過程で作った。<br>
3, 技術的ハイライト : <br>
      *SFML 3.0を用いてオブジェクト指向設計で開発。<br>
      *vcpkgを使用、依存関係をコードベースで管理。<br>
      *配布環境に合わせたArtifact作成。等<br>
4, 反省<br>
*データ構造は工夫したが、短期間、個人で開発したものなのでコードの可読性は悪い。<br>
*emsciptenを用いてwebに無理やりつなげようとしたが無理だった。<br>
4, 今後の展望<br>
     *エフェクトやフォントは整えたがscore実装などはまだなので余裕ができたら実装する。<br>
5,スクリーンショット : <br><img width="1598" height="798" alt="スクリーンショット 2026-01-06 152759" src="https://github.com/user-attachments/assets/6d9fb455-ffa7-4aeb-af0f-fa02b769a693" />
6, 使用させていただいたデータ<br>
個人学習目的で以下のフォントを使用させていただきました。<br>
https://hicchicc.github.io/00ff/<br>
1. Project Name
SFML-Tetris

2. Motivation
Bridge between Algorithms and Development:


After focusing heavily on algorithmic problem-solving via AtCoder, I decided to transition into practical software development.


This project was built as a hands-on exercise to learn the fundamentals of game architecture using modern C++, utilizing technical books and AI as learning partners.

3. Technical Highlights
Object-Oriented Design with SFML 3.0:


Developed using the latest SFML 3.0 library with a focus on object-oriented principles.

Dependency Management via vcpkg:


Utilized vcpkg (Manifest Mode) to ensure reproducible builds and clean dependency management within the repository.

Automated Artifact Packaging:


Configured CI/CD pipelines to generate distribution-ready packages, ensuring all necessary assets and DLLs are correctly bundled for the Windows environment.

4. Self-Reflection
Code Readability:


While I focused on optimizing data structures (leveraging my competitive programming background), the overall code readability remains a challenge due to the rapid, solo development process.

Web Integration (Emscripten):


I attempted to port the project to the web using Emscripten to increase accessibility. However, I encountered significant technical hurdles that I couldn't resolve within this timeframe.


This remains a key learning point for future cross-platform attempts.

5. Future Outlook
Feature Completion:


While the visual effects and font rendering are implemented, core game features like a scoring system are still pending.


I plan to implement these as my schedule allows.

6. Screenshots
<img width="1598" height="798" alt="Screenshot 2026-01-06 152759" src="https://github.com/user-attachments/assets/6d9fb455-ffa7-4aeb-af0f-fa02b769a693" />

7. Credits & Assets
For educational purposes, the following font asset was used in this project:


Font: 00FF Font by hicchicc.
