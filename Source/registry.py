import tkinter as tk
from tkinter import filedialog, messagebox
from registry_designer import RegistryApp
class RegistryProcess(RegistryApp):
    def __init__(self, nw, client, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.nw = nw  # Lưu giá trị self.nw
        self.client = client
        self.protocol("WM_DELETE_WINDOW", self.on_close)
        
    def butSend_Click(self):
        msg = "SEND_CONTENT\n"
        self.nw.write(msg)
        self.nw.flush()

        # Get the content from the txtReg text widget
        reg_content = self.txtReg.get("1.0", tk.END)

        # Send the registry content to the server
        self.nw.write(reg_content)
        self.nw.flush()   # Disable text widget again
        
        respond = self.client.recv(4096).decode()
        messagebox.showinfo("Message",  respond)
        
    def butBro_Click(self):
        file_path = filedialog.askopenfilename(filetypes=[("Reg Files", "*.reg")])
        self.txtBro.delete(0, tk.END)
        self.txtBro.insert(0, file_path)
        with open(file_path, 'r') as fin:
            content = fin.read()
            self.txtReg.delete(1.0, tk.END)  # Clear existing content
            self.txtReg.insert(tk.END, content)
        
    def button1_Click(self):
        self.nw.write("SEND\n")
        
        option = self.opApp.get()
        link = self.txtLink.get()
        value_name = self.txtNameValue.get()
        value = self.txtValue.get()
        type_value = self.opTypeValue.get()
     
        self.nw.write(option + "\n")
        self.nw.write(link + "\n")
        self.nw.write(value_name + "\n")
        self.nw.write(value + "\n")
        self.nw.write(type_value + "\n")
        self.nw.flush()
        
        response = self.client.recv(4096).decode()

        self.txtKQ.config(state=tk.NORMAL) # thiết lập trạng thái của Text widget self.txtKQ thành NORMAL, cho phép chỉnh sửa nội dung
        self.txtKQ.insert(tk.END, response + "\n") #chèn nội dung mới vào cuối của Text widget
        self.txtKQ.config(state=tk.DISABLED) #thiết lập lại trạng thái của Text widget self.txtKQ thành DISABLED, ngăn người dùng chỉnh sửa 
    
    def comboBox2_SelectedIndexChanged(self, event):
        selected_op = self.opApp.get()
        
        self.txtNameValue.place_forget()
        self.txtValue.place_forget()
        self.opTypeValue.place_forget()
            
        if selected_op == "Set value":
            self.txtNameValue.place(x=6, y=72, width=113, height=20)
            self.txtValue.place(x=125, y=72, width=138, height=20)
            self.opTypeValue.place(x=269, y=72, width=109, height=21)
        elif selected_op == "Get value" or selected_op == "Delete value":
            self.txtNameValue.place(x=6, y=72, width=113, height=20)

        
    def delete_value(self):
        self.txtBro.delete(0, tk.END)
        self.txtBro.insert(0, "Đường dẫn...")
        
        self.txtReg.delete(1.0, tk.END)
        self.txtReg.insert(tk.END, "Nội dung")
        
        self.opApp.set("Chọn chức năng")
        
        self.txtLink.delete(0, tk.END)
        self.txtLink.insert(0, "Đường dẫn")
        
        self.txtNameValue.delete(0, tk.END)
        self.txtNameValue.insert(0, "Name value")
        
        self.txtValue.delete(0, tk.END)
        self.txtValue.insert(0, "Value")
        
        self.opTypeValue.set("Type")
        
        self.txtKQ.config(state=tk.NORMAL)
        self.txtKQ.delete(1.0, tk.END)
        self.txtKQ.config(state=tk.DISABLED)
    
    def on_close(self):
        if self.client:
            self.send_quit_command()
        self.destroy()

    def send_quit_command(self):
        message = "QUIT\n"
        self.nw.write(message)
        self.nw.flush()
        self.client.close()
        self.client = None