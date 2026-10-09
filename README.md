Bài Tập Giữa Kỳ: Cài Đặt Lệnh ls (NetBSD ls(1))
Sinh viên thực hiện: Nguyễn Đình Minh Tuấn
Mã số sinh viên: 24IT295
Repository: https://github.com/T279T279/NGUYENDINHMINHTUAN_24IT295_midterm.git
Tài liệu tham khảo: Manual NetBSD 10.1 ls(1) (ls.pdf).
1. Giới thiệu đề tài
Bài tập yêu cầu hiện thực lại lệnh ls bằng ngôn ngữ C (chuẩn C99) theo đặc tả của hệ điều hành NetBSD/POSIX. Chương trình hỗ trợ 19 cờ (flags), xử lý đúng các quy tắc ghi đè cờ, duyệt đệ quy thư mục và hiển thị chi tiết thông tin file.
2. Cấu trúc thư mục dự án

Dự án được chia thành các module với thư mục `include/` chứa file header và `src/` chứa mã nguồn:

```text
NGUYENDINHMINHTUAN_24IT295_midterm/
├── .gitignore       # Bỏ qua file binary và file .o khi commit
├── Makefile         # Script tự động hóa biên dịch (make, make clean)
├── README.md        # Báo cáo và hướng dẫn sử dụng
├── include/
│   └── my_ls.h      # Khai báo cấu trúc Options, FileEntry và các hàm toàn cục
└── src/
    ├── main.c       # Hàm main điều khiển luồng chính và xử lý đối số
    ├── options.c    # Phân tích 19 cờ từ dòng lệnh và xử lý quy tắc ghi đè
    ├── format.c     # Xử lý logic định dạng hiển thị (quyền, dung lượng, thời gian)
    └── core.c       # Đọc thư mục, tính tổng số block, sắp xếp và duyệt đệ quy (-R)
3. Danh sách các cờ đã hoàn thành (19 cờ)
    1. -a: Hiển thị tất cả các file, bao gồm cả file ẩn bắt đầu bằng dấu chấm (.).

    2. -A: Hiển thị gần như tất cả các file ẩn, ngoại trừ . và ...         

    3. -c: Sử dụng thời gian thay đổi trạng thái file (ctime) khi sắp xếp hoặc hiển thị.

    4. -d: Xem thư mục như file bình thường, không liệt kê nội dung bên trong.

    5. -F: Thêm ký hiệu chỉ loại file sau tên (/ cho thư mục, * cho file thực thi, @ cho symlink).

    6. -f: Bật hiển thị file ẩn và không sắp xếp danh sách.

    7. -h: Hiển thị kích thước file ở dạng dễ đọc (B, K, M, G).

    8. -i: Hiển thị số Inode của file.

    9. -k: Tính toán số block theo đơn vị Kilobyte (1024 bytes).

    10. -l: Liệt kê chi tiết (dạng dài): quyền truy cập, số link, user, group, dung lượng, ngày sửa đổi, tên file (có in kèm dòng total tổng số block).

    11. -n: Tương tự cờ -l, nhưng hiển thị UID và GID dưới dạng số.

    12. -q: Thay thế các ký tự không in được trong tên file bằng dấu ? (mặc định trên terminal).

    13. -R: Duyệt đệ quy tất cả các thư mục con.

    14. -r: Đảo ngược thứ tự sắp xếp.

    15. -S: Sắp xếp file theo kích thước giảm dần.

    16. -s: Hiển thị số block hệ thống mà file chiếm dụng.

    17. -t: Sắp xếp file theo thời gian sửa đổi (mới nhất lên đầu).

    18. -u: Sử dụng thời gian truy cập gần nhất (atime) khi sắp xếp hoặc hiển thị.

    19. -w: In nguyên bản các ký tự trong tên file.
4. Xử lý quy tắc ghi đè cờ (Overrides)
Theo tài liệu NetBSD ls(1), khi các cờ xung đột xuất hiện cùng nhau, cờ đứng sau cùng sẽ có hiệu lực:

-w và -q: Cờ xuất hiện sau quyết định cách in tên file (bình thường hay ẩn ký tự lạ).

-l và -n: Cờ đứng sau quyết định hiển thị tên user/group hay mã số UID/GID.

-c và -u: Cờ đứng sau quyết định mốc thời gian (ctime hay atime).

-R và -d: Nếu -d đứng sau thì tắt duyệt đệ quy -R.

-k và -h: Cờ đứng sau quyết định đơn vị hiển thị dung lượng/block.
5. Hướng dẫn biên dịch và chạy thử
Biên dịch:
Sử dụng make trên terminal:

# Biên dịch ra file thực thi my_ls
make

# Xóa các file object (.o) và file thực thi
make clean
Một số lệnh kiểm thử mẫu:
# 1. Liệt kê cơ bản
./my_ls

# 2. Liệt kê chi tiết kèm file ẩn
./my_ls -la

# 3. Sắp xếp theo dung lượng giảm dần với kích thước dễ đọc
./my_ls -lhS

# 4. Hiển thị UID/GID dạng số và Inode
./my_ls -ni

# 5. Sắp xếp theo thời gian cũ nhất lên đầu
./my_ls -ltr

# 6. Duyệt đệ quy toàn bộ thư mục
./my_ls -R