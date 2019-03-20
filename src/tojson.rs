use std::io::{Read, Write, BufReader, BufRead, Error};

use crate::StringUtils;
use crate::error::*;

#[derive(PartialEq)]
enum Entry {
    ARRAY,
    CLASS,
    PROPERTY,
}

pub fn tojson<I: Read, W: Write>(input: I, mut output: W) -> Result<(), Error>{
    output.write(b"{{")?;

    let mut last: Entry = Entry::CLASS;
    let mut empty_class = false;
    let mut class_end = false;

    let buffer = BufReader::new(input);
    let mut lines = buffer.lines();
    while let Some(line) = lines.next() {
        let l = line.unwrap()
            .replace("\\", "\\\\")
            .replace("\t", "");
        let l = l.trim().to_owned();
        if !l.starts_with("//") && !l.starts_with("#") {
            if l.starts_with("class") {
                if last == Entry::PROPERTY || last == Entry::ARRAY {
                    output.write(b",")?;
                }
                if empty_class && class_end {
                    output.write(b",")?;
                }
                if l.ends_with("{};") {
                    output.write_fmt(format_args!("\"{}\":{{}}", l.substring(6, l.len() - (
                        if l.ends_with(" {};") { 10 } else { 9 }
                    ))))?;
                    class_end = true;
                } else {
                    if l.ends_with("{") {
                        output.write_fmt(format_args!("\"{}\":{{", l.substring(6, l.len() - (
                            if l.ends_with(" {") { 8 } else { 7 }
                        ))))?;
                    } else {
                        output.write_fmt(format_args!("\"{}\":", l.substring(6, l.len() - 6)))?;
                    }
                    class_end = false;
                }
                empty_class = true;
                last = Entry::CLASS;
            } else {
                match l.find("=") {
                    Some(i) => {
                        empty_class = false;
                        if last != Entry::CLASS || (last == Entry::CLASS && class_end) {
                            output.write(b",")?;
                        }
                        match l.find("[]") {
                            Some(v) if v < i => {
                                // Array
                                output.write_fmt(format_args!("\"{}\":", l.substring(0, v)))?;
                                let value: String = match l.find("};") {
                                    Some(_) => {
                                        let value = l.substring(
                                            i + 2,
                                            l.len() - i - match l.find("= {") {
                                                Some(_) =>  3,
                                                None => 2
                                        });
                                        let value = value.replace("{", "[").replace("}", "]");
                                        read_quoted(value)
                                    }
                                    None => {
                                        let mut done = false;
                                        let mut val = String::new();
                                        while !done {
                                            let line = lines.next().unwrap().unwrap();
                                            done = match line.find("};") {
                                                Some(_) => true,
                                                None => false
                                            };
                                            if !done {
                                                let value = line.trim().to_owned();
                                                if value == "{" {
                                                    continue;
                                                }
                                                val = read_quoted(value);
                                            }
                                        }
                                        val
                                    }
                                };
                                output.write_fmt(format_args!("{}", value))?;
                                last = Entry::ARRAY;
                            }
                            _ => {
                                // Property
                                let (mut value, property) = match l.find(" = ") {
                                    Some(p) if p + 1 == i => {
                                        (
                                            l.substring(i + 2, l.len() - i - 3).to_owned(),
                                            l.substring(0, i - 1)
                                        )
                                    },
                                    _ => {
                                        (
                                            l.substring(i + 1, l.len() - i - 2).to_owned(),
                                            l.substring(0, i)
                                        )
                                    }
                                };
                                while value.find("\" \\\\n \"") != None {
                                    value = value.replace("\" \\\\n \"", "\\n");
                                }
                                if value != "\"\"" {
                                    if value.starts_with("\"\"\"") {
                                        value = value.substring(1, value.len() - 1).to_owned();
                                        value = value.replace("\"\"", "\\\"\\\"");
                                        value = String::from("\"") + &value;
                                    } else {
                                        value = value.replace("\"\"", "\\\"\\\"");
                                    }
                                }
                                output.write_fmt(format_args!("\"{}\":{}", property, value))?;
                                last = Entry::PROPERTY;
                            }
                        }
                    }
                    None => {
                        if l == "};" {
                            output.write(b"}}")?;
                        } else {
                            output.write_fmt(format_args!("{}", l))?;
                        }
                    }
                }
            }
        }
    }
    output.write(b"}}\n")?;
    Ok(())
}

fn read_quoted(s: String) -> String {
    let mut string_open = false;
    let mut chars = s.chars().peekable();
    let mut new_value: Vec<u8> = Vec::new();
    while let Some(c) = chars.next() {
        if string_open {
            if c == '"' {
                if chars.peek() == Some(&'"') {
                    "\\\"\\\"".chars().for_each(|x| new_value.push(x as u8));
                    chars.next();
                    continue;
                } else {
                    string_open = false;
                }
            }
        } else {
            if c == '"' {
                string_open = true;
            }
        }
        new_value.push(c as u8);
    }
    String::from_utf8(new_value).unwrap_or_print()
}
