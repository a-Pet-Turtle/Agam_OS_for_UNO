import tkinter as tk
from tkinter import scrolledtext

root = tk.Tk()
root.title("Character Builder for Arduino C++")

# Mode selection
mode_var = tk.StringVar(value="single")

def switch_mode():
    if mode_var.get() == "single":
        show_single_mode()
    elif mode_var.get() == "3x2":
        show_multi_mode(3, 2)
    else:  # 4x2
        show_multi_mode(4, 2)

# Single character mode (5x8)
def show_single_mode():
    for widget in char_frame.winfo_children():
        widget.destroy()
    
    global single_checkboxes
    single_checkboxes = []
    
    for row in range(8):
        row_checks = []
        frame = tk.Frame(char_frame)
        frame.pack()
        for col in range(5):
            var = tk.IntVar()
            cb = tk.Checkbutton(frame, variable=var, width=2)
            cb.pack(side=tk.LEFT)
            row_checks.append(var)
        single_checkboxes.append(row_checks)

# Multi character mode
def show_multi_mode(cols, rows):
    for widget in char_frame.winfo_children():
        widget.destroy()
    
    global multi_checkboxes, multi_cols, multi_rows
    multi_cols = cols
    multi_rows = rows
    multi_checkboxes = []
    
    total_pixel_rows = rows * 8
    total_pixel_cols = cols * 5
    
    # Create rows
    for row in range(total_pixel_rows):
        row_checks = []
        frame = tk.Frame(char_frame)
        frame.pack()
        
        # Add horizontal separator lines between character rows
        if row > 0 and row % 8 == 0:
            sep = tk.Frame(char_frame, height=2, bg="gray")
            sep.pack(fill=tk.X, pady=2)
        
        for col in range(total_pixel_cols):
            # Add vertical separator lines between character columns
            if col > 0 and col % 5 == 0:
                tk.Frame(frame, width=2, bg="gray").pack(side=tk.LEFT)
            
            var = tk.IntVar()
            cb = tk.Checkbutton(frame, variable=var, width=2)
            cb.pack(side=tk.LEFT)
            row_checks.append(var)
        multi_checkboxes.append(row_checks)

def build_char():
    result_text.delete(1.0, tk.END)
    
    if mode_var.get() == "single":
        # Single character output
        lines = []
        for row in single_checkboxes:
            binary = ''.join(str(var.get()) for var in row)
            lines.append(f"  0b{binary}")
        
        output = "byte customChar[8] = {\n" + ",\n".join(lines) + "\n};"
        result_text.insert(1.0, output)
    
    else:
        # Multi character output
        num_chars = multi_cols * multi_rows
        
        if multi_cols == 3:
            char_names = ["logo1", "logo2", "logo3", "logo4", "logo5", "logo6"]
        else:  # 4x2
            char_names = ["logo1", "logo2", "logo3", "logo4", 
                         "logo5", "logo6", "logo7", "logo8"]
        
        all_output = []
        
        for char_idx in range(num_chars):
            # Determine which rows and columns for this character
            char_row = char_idx // multi_cols
            char_col = char_idx % multi_cols
            
            start_row = char_row * 8
            start_col = char_col * 5
            
            lines = []
            for row in range(8):
                actual_row = start_row + row
                binary = ''.join(
                    str(multi_checkboxes[actual_row][start_col + col].get()) 
                    for col in range(5)
                )
                lines.append(f"  0b{binary}")
            
            char_output = f"byte {char_names[char_idx]}[8] = {{\n" + ",\n".join(lines) + "\n};"
            all_output.append(char_output)
        
        result_text.insert(1.0, "\n\n".join(all_output))

# Mode selector frame
mode_frame = tk.Frame(root)
mode_frame.pack(pady=5)

tk.Label(mode_frame, text="Mode:").pack(side=tk.LEFT)
tk.Radiobutton(mode_frame, text="Single (5x8)", variable=mode_var, 
               value="single", command=switch_mode).pack(side=tk.LEFT)
tk.Radiobutton(mode_frame, text="3x2 (15x16)", variable=mode_var, 
               value="3x2", command=switch_mode).pack(side=tk.LEFT)
tk.Radiobutton(mode_frame, text="4x2 (20x16)", variable=mode_var, 
               value="4x2", command=switch_mode).pack(side=tk.LEFT)

# Character drawing frame
char_frame = tk.Frame(root)
char_frame.pack(pady=10)

# Build button
build_button = tk.Button(root, text="Build Character", command=build_char, 
                         font=("Arial", 12, "bold"))
build_button.pack(pady=5)

# Result text
result_text = scrolledtext.ScrolledText(root, height=35, width=50, font=("Courier", 9))
result_text.pack(padx=10, pady=10)

# Initialize with single mode
show_single_mode()

root.mainloop()
