# City's Corpse: Bài tập Lập trình Nâng cao INT2215



## I. Thông tin tác giả 
- **Họ và tên:** Hoàng Văn Thái 
- **Mã số sinh viên:** 24021621
- **Lớp:** K69I - CS1

## II. Tổng quan 
- **Thể loại:** Infinite Loops.
- **Nền tảng:** PC.
- **Ngôn ngữ / Thư viện sử dụng:** C++ / SDL2. 
- **Tóm tắt:** Trong _City's Corpse_, người chơi sẽ phải dùng chuột điều khiển nòng súng của một chiếc xe tăng đang di chuyển trong trong thành phố để tiêu diệt các **zombie**. Tuỳ vào số **zombie** người chơi hạ gục được thì độ khó sẽ thay đổi, càng nhiều mạng hạ gục thì độ khó càng cao. Trong quá trình chơi, người chơi có thể nhận được các **Hiệu ứng đặc biệt** như nhân đôi điểm hạ gục, nhân đôi tiền, hoặc là bất ngờ xuất hiện 1 bầy zombie. Cùng với đó game sẽ cung cấp cho người chơi các kĩ năng nhằm hỗ trợ họ có thể đạt được số điểm cao nhất có thể.
- **Ngưỡng điểm:** 9 ~ 10 - ... .

## III. Các điểm nổi bật của Game.
### 1. Thuật toán sinh ngẫu nhiên zombie & Độ khó

Game sử dụng thuật toán sinh bản sao từ hình ảnh zombie gốc.

- **Càng hạ gục nhiều zombie**, **tốc độ zombie càng tăng**.
- Điều này giúp tạo ra một nhịp độ chơi gấp gáp, đồng thời tăng dần độ khó cho người chơi trong suốt hành trình.

---

### 2. Thuật toán tìm kiếm vị trí gần nhất giữa zombie và xe tăng & Áp dụng vật lí

- Để tạo cho game cảm giác trân thật khi đang di chuyển trên đường bằng áp dụng vật lí.
- Ứng dụng hình học nhằm tìm ra khoảng cách đi đến xe tăng gần nhất của zombie.

---

### 3. Nâng cấp đa dạng
Có toàn bộ **3 loại hiệu ứng** cho người chơi, được sinh ngẫu nhiên trong trận đấu. Các loại nâng cấp: 

- Số điểm nhận được tăng lên gấp 2 lần.
- Số tiền nhận được tăng lên gấp 2 lần.
- Spawn 1 bầy gồm 5 con zombie tại vị trí ngẫu nhiên.

---

### 4. Thuật toán vẽ đường tròn (Midpoint Circle Algorithm)

Bằng cách chia hình tròn sẽ được vẽ ra làm 2 phần bằng nhau ở các góc phần tư và lần lượt lấy các điểm trong 8 cung tròn đó để vẽ tạo thành đường tròn.
- **Link tìm hiểu:** [Midpoint Circle Algorithm](https://en.wikipedia.org/wiki/Midpoint_circle_algorithm).

---

### 5. Hệ thống Animation đa dạng 

- **Hiệu ứng quay trục súng:** Dễ dàng kiểm soát, điều khiển. Hình ảnh chuyển động mượt mà.
- **Hiệu ứng cuộn vô hạn của Background:** Tạo cảm giác không gian rộng mở và chuyển động liền mạch trong suốt quá trình chơi.

---
  
### 6. Hệ thống Âm thanh 

- Âm nhạc nền 
- Âm thanh cho các nút: Hover, Click
- Âm thanh cho người chơi nhận nâng cấp
- Âm thanh bị quái vật va trúng

---

### 7. Chức năng Lưu bảng điểm xếp hạng (Highscore). 

**Lưu trữ thông tin Highscore trong file**: `highscore.txt`.  
_Sau khi hoàn thành lượt chơi_, kết quả sẽ được ghi vào file.

Người chơi có thể xem được **5 điểm** cao nhất.

---

### 8. Chức năng Tiền, Cửa hàng và Chiêu thức bổ trợ (Skill)

#### Tiền (Coin)

- Với mỗi **1 Zombie** mà người chơi hạ gục thì sẽ cung cấp cho người chơi **1 Coin**.
- Tuỳ vào số lượng Coin mà người chơi đang có mà có thể sở hữu cho mình các Chiêu thức nhằm gia tăng điểm số cao hơn.

#### Chiêu thức (Skill)

- **Skill 1: Piercing** là đạn xuyển thấu giúp người chơi có thể bắn xuyên zombie.
- **Skill 2: ShockWave** tạo ra 1 vòng tròn giúp phá vỡ vòng vây của zombie. 

## IV. Tài liệu tham khảo
- Tham khảo Code mẫu trên [Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL/index.php).
- Tham khảo Code mẫu trên [File tài liệu do giảng viên cung cấp](https://lazyfoo.net/tutorials/SDL/index.php).
- Tham khảo Code mẫu trên [trang chính thức của SDL2](https://www.libsdl.org/).
- Một số hình ảnh được lấy từ [itch.io](https://itch.io/) và Sora (Chat GPT). 
- hiệu ứng âm thanh được từ [Pixabay](https://pixabay.com) và [Myinstants](https://www.myinstants.com/fr/index/vn/).

Ngoài ra, không sử dụng code ở những nguồn không được liệt kê ở trên
