# Bắn Máy Bay (Space Invaders) - STM32F429I-DISCO & TouchGFX
Dự án game bắn máy bay cổ điển (Space Invaders) được thiết kế giao diện bằng phần mềm **TouchGFX** và chạy trên vi điều khiển **STM32F429ZIT6** (Kit phát triển STM32F429I-DISCO). Người chơi sẽ điều khiển máy bay thông qua 5 nút bấm vật lý gắn ngoài.
---
## 🛠️ Yêu cầu phần cứng (Hardware Requirements)
1. **Board mạch:** Kit phát triển **STM32F429I-DISCO** (màn hình cảm ứng TFT 2.4 inch).
2. **Nút bấm:** 5 nút bấm vật lý (Push button) loại 2 chân hoặc 4 chân.
3. **Dây cắm:** Dây cắm testboard (Female-to-Male hoặc Male-to-Male tùy theo nút).
4. **Cáp kết nối:** Cáp Mini-USB (chuẩn cũ) cắm vào cổng ST-LINK phía trên đỉnh board để nạp code và cấp nguồn.
## 💻 Yêu cầu phần mềm (Software Requirements)
- **STM32CubeIDE:** Trình biên dịch và nạp code chính (Khuyến nghị bản 1.14.0 trở lên).
- **TouchGFX Designer:** Phần mềm thiết kế giao diện game (cùng version với bản cài trong CubeIDE).
- **STM32CubeMX:** (Tùy chọn) Để cấu hình lại chân I/O nếu cần thiết.
---
## 🔌 Hướng dẫn đấu nối phần cứng (Wiring)
Mạch STM32F429I-DISCO có rất nhiều linh kiện được tích hợp sẵn (Màn hình LCD, Cảm biến Gyro, RAM SDRAM, IC USB...). Do đó, việc chọn chân cho nút bấm phải cực kỳ cẩn thận để **không bị xung đột** với các linh kiện này (Ví dụ: Tuyệt đối không dùng PA1, PA2 vì trùng với ngắt của Gyro, không dùng PA5 vì trùng báo lỗi USB).
Sơ đồ nối dây đã được tối ưu và thiết lập sẵn trong code (Sử dụng điện trở kéo lên - Pull-Up nội bộ của chip):
- Mỗi nút bấm vật lý sẽ có 2 đầu nối.
- **Một đầu của tất cả 5 nút** được đấu chụm lại và nối vào chân **GND** (Mass) trên bo mạch.
- Đầu còn lại của từng nút được nối vào các chân sau:
|
 Nút bấm trong Game 
|
 Chức năng 
|
 Chân trên STM32 (Vị trí) 
|
|
:---:
|
:---:
|
:---
|
|
**
W
**
|
 Đi Lên 
|
**
PA7
**
 (Hàng rào P1, Chân số 30) 
|
|
**
S
**
|
 Đi Xuống 
|
**
PC8
**
 (Hàng rào P2, Chân số 29) 
|
|
**
A
**
|
 Sang Trái 
|
**
PC4
**
 (Hàng rào P2, Chân số 15) 
|
|
**
D
**
|
 Sang Phải 
|
**
PC5
**
 (Hàng rào P2, Chân số 13) 
|
|
**
SPACE
**
|
 Bắn Đạn 
|
**
PA10
**
 (Hàng rào P1, Chân số 43) 
|
> **Lưu ý:** Xem kỹ ghi chú tên chân (Silkscreen) được in ở mặt sau của bo mạch để cắm cho chuẩn.
---
## 🚀 Hướng dẫn biên dịch và nạp code (Build & Run)
1. Mở phần mềm **STM32CubeIDE**.
2. Chọn `File -> Open Projects from File System...` và trỏ đến thư mục gốc của dự án này.
3. Mở rộng cây thư mục bên tay trái (Project Explorer), chọn Project `STM32F429I_DISCO_REV_D01`.
4. Click chuột phải vào Project -> Chọn **Clean Project** (Xóa bộ nhớ đệm cũ).
5. Click nút **Build** (Biểu tượng cái búa 🔨) trên thanh công cụ và chờ đến khi Console báo `Build Finished. 0 errors, 0 warnings`.
6. Cắm cáp kết nối bo mạch với máy tính.
7. Click nút **Run** (Biểu tượng nút Play xanh lá ▶️) để nạp code xuống mạch.
8. Chờ mạch khởi động lại, màn hình game hiện lên và bắt đầu bấm nút chơi!
*(Độ trễ phím (Debounce) đã được tối ưu hóa xuống mức 1 chu kỳ ~ 16ms, đem lại cảm giác di chuyển tức thời không độ trễ, cực mượt).*
---
## 🎨 Hướng dẫn chỉnh sửa giao diện / Hình ảnh game
Toàn bộ đồ họa và hình ảnh của game được quản lý bởi TouchGFX. Để đổi ảnh nền, thêm máy bay, thay font chữ:
1. Mở thư mục dự án, vào đường dẫn: `TouchGFX/`
2. Click đúp vào file `STM32F429I_DISCO_REV_D01.touchgfx` để mở phần mềm TouchGFX Designer.
3. Chỉnh sửa, thiết kế UI tùy ý.
4. (Quan trọng) Chuyển định dạng các hình ảnh lớn (như Background) sang `RGB565` hoặc `L8_RGB565` để giảm dung lượng. 
5. Ấn nút **Generate Code** (phía trên cùng bên phải) trong TouchGFX Designer.
6. Tắt TouchGFX, quay lại **STM32CubeIDE**.
7. Bấm phím **F5** (Refresh) để cập nhật file mới, sau đó ấn **Build** và **Run**.
---
## ⚠️ Khắc phục sự cố thường gặp (Troubleshooting)
### 1. Lỗi tràn bộ nhớ: `region 'RAM' overflowed` hoặc `region 'FLASH' overflowed`
- **Nguyên nhân:** Các ảnh (.png, .jpg) chưa được tối ưu hoặc dung lượng game vượt quá giới hạn 2MB Internal Flash. Nếu tràn RAM, nguyên nhân có thể do mất cấu hình Linker nhận diện SDRAM ngoài.
- **Khắc phục tràn Flash:** Vào TouchGFX, xóa bớt ảnh không dùng trong thư mục `assets/images`. Chọn tất cả ảnh chuyển Format sang `L8_ARGB8888` hoặc `RGB565`. 
- **Khắc phục tràn RAM:** Khi bạn sửa mạch bằng CubeMX, phần mềm tự sinh ra file Linker mới đè lên file Linker cấu hình chuẩn. Hãy copy thủ công nội dung file `STM32F429XX_FLASH.ld` từ thư mục gốc của dự án và dán đè vào file `STM32CubeIDE/STM32F429ZITX_FLASH.ld`.
### 2. Lỗi Build: `multiple definition of ...`
- **Khắc phục:** Nguyên nhân do STM32CubeIDE chưa xóa đi các file thư viện rác cũ sau khi sinh code mới. Dọn dẹp bằng cách click chuột phải vào Project -> Chọn **Clean Project** rồi Build lại.
### 3. Ấn nút di chuyển không ăn, hoặc tự động trôi
- **Khắc phục:** 100% do cắm nhầm dây hoặc cắm vào các chân đang bị xung đột (VD: PA1, PA2, PA5). Hãy cắm lại chuẩn theo mục đấu dây ở trên và thử lại.
 
