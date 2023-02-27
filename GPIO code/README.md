
ĐỀ BÀI:
Cho sơ đồ mạch bao gồm Rasp, Led RGB và Module 4 nút nhấn.

Bài 1: Viết code để các đèn led R,G,B lần lượt sáng dần rồi tối dần. Thời gian sáng dần và tối dần là 2s.

Bài 2: Viết code để 4 nút nhấn điều khiển độ sáng của đèn, với BT1 -> sáng yếu nhất, BT4 sáng nhất.

Bài 3: Viết code để nút BT1 có chức năng chọn màu hiển thị, BT2 tăng độ sáng của màu hiển thị, BT3
giảm độ sáng của màu hiển thị.

Bài 4: Ngẫu nhiên một trong 3 đèn R,G,B sáng mỗi 3s. Nếu trong 500ms người chơi kịp nhấn nút tương
ứng thì đèn led sẽ tắt và cả 3 đèn sẽ chớp nháy trong 1s. Nếu người chơi không kịp nhấn nút thì đèn tắt.
Kết quả sẽ được cập nhập trên terminal sau mỗi lần đèn sáng.
Với (BT1 -> R; BT2 ->G; BT3 -> B)

Bài 5: Ban đầu cả 3 led đều tắt. Ngẫu nhiên led R sáng trong 1s rồi tắt. Người chơi được yêu cầu nhấn
BT1 ngay sau khi led R sáng. Nếu người chơi kịp nhấn trong 1s, led R sẽ tắt, terminal hiển thị thời gian từ
lúc led R sáng cho đến khi BT1 được nhấn. Nếu người chơi không kịp nhấn, đèn đỏ sẽ tắt sau 1s, và terminal hiển thị thông báo “...”. Sau khi led R tắt từ 1-3s, led R sẽ ngẫu nhiên sáng trở lại. Quá trình này lặp đi lặp lại 10 lần. Sau 10 lần terminal hiển thị thời gian nhấn trung bình của người chơi.
