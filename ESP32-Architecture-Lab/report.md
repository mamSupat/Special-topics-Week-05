## คำถามทบทวน
1. Docker Commands: คำสั่ง docker-compose up -d และ docker-compose exec esp32-dev bash ทำอะไร?

ตอบ docker-compose up -d รัน container ทั้งหมดแบบเบื้องหลัง

docker-compose exec esp32-dev bash เข้า shell ของ container esp32-dev

2. ESP-IDF Tools: เครื่องมือไหนจาก Lab4 ที่จะใช้ในการ build โปรแกรม ESP32?

ตอบ ใช้คำสั่ง idf.py build

3. New Tools: เครื่องมือใหม่ที่ติดตั้ง (tree, htop) ใช้ทำอะไร?

ตอบ tree แสดงโครงสร้างไฟล์แบบต้นไม้ / htop ดูการใช้ CPU และ RAM แบบเรียลไทม์

4. Architecture Focus: การศึกษา ESP32 architecture แตกต่างจากการทำ arithmetic ใน Lab4 อย่างไร?

ตอบ ESP32 Architecture ศึกษาโครงสร้างภายในของชิปและระบบ multitasking ต่างจาก Lab4 ที่เน้นการคำนวณเชิงโปรแกรม

## คำถามวิเคราะห์
1. Memory Types: SRAM และ Flash Memory ใช้เก็บข้อมูลประเภทไหน?

ตอบ SRAM เก็บตัวแปรที่เปลี่ยนแปลงได้ (global, stack, heap)

Flash เก็บข้อมูลคงที่และโค้ดโปรแกรม

2. Address Ranges: ตัวแปรแต่ละประเภทอยู่ใน address range ไหน?

ตอบ SRAM ≈ 0x3FFB0000 ขึ้นไป

Flash ≈ 0x3F400000 ขึ้นไป

3. Memory Usage: ESP32 มี memory ทั้งหมดเท่าไร และใช้ไปเท่าไร?

ตอบ ESP32 มี SRAM ≈ 520 KB และ Flash ≈ 4 MB ใช้จริงประมาณ 30–300 KB ขึ้นอยู่กับโปรแกรม

## คำถามวิเคราะห์
1. Core Specialization: จากผลการทดลอง core ไหนเหมาะกับงานประเภทใด?

ตอบ Core 0 เหมาะกับงานระบบ, interrupt, Wi-Fi/BLE

Core 1 เหมาะกับงานคำนวณหนักและ task ยาว

2. Communication Overhead: inter-core communication มี overhead เท่าไร?

ตอบ การสื่อสารข้ามคอร์มี overhead ประมาณ 3–20 µs ต่อการส่ง (queue/notify)

3. Load Balancing: การกระจายงานระหว่าง cores มีประสิทธิภาพหรือไม่?

ตอบ กระจายงานได้ดีถ้าไม่แย่งหน่วยความจำ speedup ~1.7–1.9×

งานที่ชน bus หรือ cache มาก ประสิทธิภาพลดเหลือ ~1.2×