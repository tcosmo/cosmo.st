import os, errno
from pathlib import Path

import frontmatter
import jinja2
import markdown

from datetime import datetime

SITE_FOLDER = "_site"
POSTS_FOLDER = "posts"
PAGES_FOLDER = "pages"
LAYOUT_NAME = "__layout.html"

templateLoader = jinja2.FileSystemLoader(searchpath=".")
templateEnv = jinja2.Environment(loader=templateLoader)
templateEnv.filters["markdown"] = lambda content: markdown.markdown(
    content, extensions=["fenced_code", "footnotes", "md_in_html", "toc", "sane_lists"]
)

print("Generating website")

posts = {}
for root, d_names, f_names in os.walk(POSTS_FOLDER):
    for f_name in f_names:
        filename, file_extension = os.path.splitext(f_name)
        if file_extension == ".md":
            path = os.path.join(root, f_name)
            path_no_ext = os.path.join(root, filename)
            post = frontmatter.load(path)
            posts[path_no_ext] = post

posts_ordered = [
    (post_path + ".html", post)
    for post_path, post in sorted(posts.items(), key=lambda x: x[1].metadata["date"])[
        ::-1
    ]
]

pages = {}

for elem in os.listdir(PAGES_FOLDER):
    if elem == LAYOUT_NAME:
        continue
    template = templateEnv.get_template(os.path.join(PAGES_FOLDER, elem))
    output = template.render(posts_ordered=posts_ordered)

    pages[elem] = output


def make_dirs_if_not_exist(path):
    if not os.path.exists(os.path.dirname(path)):
        try:
            os.makedirs(os.path.dirname(path))
        except OSError as exc:  # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise


def get_last_edited(post_path):
    last_modified = os.path.getmtime(post_path + ".md")
    return datetime.fromtimestamp(last_modified).isoformat()


def generate_site(pages, posts):
    Path(SITE_FOLDER).mkdir(exist_ok=True)

    for page in pages:
        path = os.path.join(SITE_FOLDER, page)
        make_dirs_if_not_exist(path)
        with open(path, "w") as f:
            f.write(pages[page])

    post_template = templateEnv.get_template("posts/post.html")
    for post in posts:
        path = os.path.join(SITE_FOLDER, post + ".html")
        make_dirs_if_not_exist(path)

        last_edited = get_last_edited(post)
        output = post_template.render(
            post_content=posts[post].content, last_edited=last_edited
        )
        with open(path, "w") as f:
            f.write(output)

    return


generate_site(pages, posts)
